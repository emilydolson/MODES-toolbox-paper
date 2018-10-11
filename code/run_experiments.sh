

for filename in $@
do
    cp nk_oee $filename
    cd $filename
    if [ ! -f run.log ]; then
        ./command.sh >> run.log
    fi
    cd ../..
    echo $filename
done