

for filename in $@
do
    cp nk_oee $filename
    cd $filename
    ./command.sh >> run.log
    cd ../..
    echo $filename
done