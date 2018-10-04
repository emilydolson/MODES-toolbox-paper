//  Copyright (C) Emily Dolson 2018.
//  Released under the MIT Software license; see doc/LICENSE
//
//
//  This file contains the bulk of the code for studying open-ended evolution in NK Landscapes

#include <iostream>

#include "config/ArgManager.h"
#include "Evolve/NK.h"
#include "Evolve/World.h"
#include "tools/BitVector.h"
#include "tools/Random.h"
#include "tools/sequence_utils.h"
#include "Evolve/OEE.h"
#include "tools/vector_utils.h"


EMP_BUILD_CONFIG( NKConfig,
  GROUP(MAIN, "Global settings for NK model"),
  VALUE(K, uint32_t, 10, "Level of epistasis in the NK model"),
  VALUE(N, uint32_t, 200, "Number of bits in each organisms (must be > K)"), ALIAS(GENOME_SIZE),
  VALUE(SEED, int, 0, "Random number seed (0 for based on time)"),
  VALUE(POP_SIZE, uint32_t, 1000, "Number of organisms in the popoulation."),
  VALUE(MAX_GENS, uint32_t, 2000, "How many generations should we process?"),
  VALUE(MUT_RATE, double, .005, "Probability of each site being mutated."),
  VALUE(CHANGE_RATE, uint32_t, 100000, "How frequently should the environment change?"),

  GROUP(SELECTION_METHODS, "Settings related to selection"),
  VALUE(SELECTION, uint32_t, 0, "Selection method. 0 = Tournament, 1 = fitness sharing, 2 = lexicase, 3 = Eco-EA, 4 = Random"),
  VALUE(TOURNAMENT_SIZE, int, 2, "For tournament selection, number of individuals to include in tournament"),

  GROUP(OPEN_ENDED_EVOLUTION, "Settings related to tracking MODES metrics"),
  VALUE(MODES_RESOLUTION, int, 1, "How often should MODES metrics be calculated?"),
  VALUE(FILTER_LENGTH, int, 1000, "How many generations should we use for the persistence filter?"),
  VALUE(SHARING_THRESHOLD, double, 50, "How similar do individuals need to be before they compete under fitness sharing?"),
  VALUE(SHARING_ALPHA, double, 1, "Alpha controls the shape of the sharing function under fitness sharing?")
);

using BitOrg = emp::BitVector;

class NKWorld : public emp::World<BitOrg> {

    enum class SELECTION_METHOD { TOURNAMENT=0, SHARING=1, LEXICASE=2, ECOEA=3, RANDOM=4 };

    uint32_t N;
    uint32_t K;
    uint32_t POP_SIZE;
    uint32_t MAX_GENS;
    uint32_t MUT_RATE;
    uint32_t SELECTION;
    uint32_t CHANGE_RATE;
    int TOURNAMENT_SIZE;
    int MODES_RESOLUTION;
    int FILTER_LENGTH;
    double SHARING_ALPHA;
    double SHARING_THRESHOLD;

    emp::NKLandscape landscape;
    emp::Ptr<emp::OEETracker<BitOrg, BitOrg, emp::vector<int>>> oee;
    emp::DataFile oee_file;

    public:

    NKWorld() : oee_file("oee.csv") {;}
    NKWorld(emp::Random & rnd) : emp::World<BitOrg>(rnd), oee_file("oee.csv") {;}
    ~NKWorld(){if (oee){oee.Delete();}}

    void Setup(NKConfig & config) {
        N = config.N();
        K = config.K();
        POP_SIZE = config.POP_SIZE();
        MAX_GENS = config.MAX_GENS();
        MUT_RATE = config.MUT_RATE();
        SELECTION = config.SELECTION();
        TOURNAMENT_SIZE = config.TOURNAMENT_SIZE();
        MODES_RESOLUTION = config.MODES_RESOLUTION();
        FILTER_LENGTH = config.FILTER_LENGTH();
        SHARING_THRESHOLD = config.SHARING_THRESHOLD();
        SHARING_ALPHA = config.SHARING_ALPHA();
        CHANGE_RATE = config.CHANGE_RATE();

        landscape = emp::NKLandscape(N, K, *random_ptr);

        std::function<double(BitOrg&)> fit_fun =
            [this](BitOrg & org){ return landscape.GetFitness(org); };

        if (SELECTION == (uint32_t) SELECTION_METHOD::SHARING) {
            SetSharedFitFun(fit_fun, [](BitOrg & org1, BitOrg & org2){return calc_hamming_distance(org1, org2);}, SHARING_THRESHOLD, SHARING_ALPHA);
        } else {
            SetFitFun(fit_fun);
        }

        emp::Ptr<emp::Systematics<BitOrg, BitOrg, emp::datastruct::oee_data<emp::vector<int>>>> sys;
        sys.New([](const BitOrg & o){return o;});
        oee.New(sys, [fit_fun](BitOrg & org){return emp::Skeletonize(org, fit_fun);}, [](const emp::vector<int> & org){
            return org.size() - emp::Count(org, -1);
        });
        oee->SetResolution(MODES_RESOLUTION);
        oee->SetGenerationInterval(FILTER_LENGTH);
        AddSystematics(sys);
        OnUpdate([this](int ud){oee->Update(ud); oee_file.Update(ud);});
        OnUpdate([this](int ud){if (emp::Mod(ud, CHANGE_RATE) == 0) {landscape = emp::NKLandscape(N, K, *random_ptr);}});

        SetupFitnessFile().SetTimingRepeat(10);
        SetupSystematicsFile().SetTimingRepeat(10);
        SetupPopulationFile().SetTimingRepeat(10);
        SetPopStruct_Mixed(true);
        SetSynchronousSystematics(true);
        SetCache();

        oee_file.AddVar(update, "generation", "Generation");
        oee_file.AddCurrent(*oee->GetDataNode("change"), "change", "change potential");
        oee_file.AddCurrent(*oee->GetDataNode("novelty"), "novelty", "novelty potential");
        oee_file.AddCurrent(*oee->GetDataNode("diversity"), "ecology", "ecology potential");
        oee_file.AddCurrent(*oee->GetDataNode("complexity"), "complexity", "complexity potential");
        oee_file.PrintHeaderKeys();
        oee_file.SetTimingRepeat(MODES_RESOLUTION);

        // Build a random initial population
        for (uint32_t i = 0; i < POP_SIZE; i++) {
            BitOrg next_org(N);
            for (uint32_t j = 0; j < N; j++) next_org[j] = random_ptr->P(0.5);
            Inject(next_org);
        }

        // Setup the mutation function.
        std::function<size_t(BitOrg &, emp::Random &)> mut_fun =
            [this](BitOrg & org, emp::Random & random) {
                size_t num_muts = 0;
                for (uint32_t m = 0; m < N; m++) {
                    if (random_ptr->P(MUT_RATE)) {
                        org[m] = random_ptr->P(.5); // Randomly assign 0 or 1 
                        num_muts++;
                    }
                }
                return num_muts;
            };
        SetMutFun( mut_fun );
        SetAutoMutate();

    }

    void RunStep() {

        std::cout << update << std::endl;
        switch(SELECTION) {
            case (uint32_t)SELECTION_METHOD::TOURNAMENT :
               emp::TournamentSelect(*this, TOURNAMENT_SIZE, POP_SIZE);
               break;

            case (uint32_t)SELECTION_METHOD::SHARING : // Sharing is handled in the setting of the fitness function
               emp::TournamentSelect(*this, TOURNAMENT_SIZE, POP_SIZE);
               break;

            case (uint32_t)SELECTION_METHOD::RANDOM :
                emp::RandomSelect(*this, POP_SIZE);
                break;
            // case SELECTION_METHOD::LEXICASE :
            //     emp::LexicaseSelect(*this, fit_set, POP_SIZE);
            //      break;
            default:
                emp_assert(false && "INVALID SELECTION SCEHME", SELECTION);
                break;
        }
        Update();
    }

    void Run() {
        for (size_t u = 0; u <= MAX_GENS; u++) {
            RunStep();
        }  
    }

};