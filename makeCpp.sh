while sleep 1;
    do watch -d -t -g "ls -l --full-time | grep -G crcExperiments.cpp" && make && echo "make successful at $(date +%r)";
done;






















