#!/bin/bash

#if [-f badruns_all_target.txt] then
#	rm badruns_all_target.txt
#fi

#if [-f badruns_all_targetv2.txt] then
#	rm badruns_all_targetv2.txt
#fi
find /pnfs/e906/scratch/users/apun/e906-root-ana/target_all/01/*/*/out/  -type d '!' -exec test -e "{}/output.root" ';' -print >>badruns_all_target.txt

#find /pnfs/e906/scratch/users/apun/e906-root-ana/LD2_target/01/*/*/out/  -type d '!' -exec test -e "{}/output.root" ';' -print >>badruns_LD2_target.txt

while read -r line
do 
  #dirc=echo "$line"
  dirname $line >> badruns_all_targetv2.txt
  #dirname $line >> badruns_LD2_targetv2.txt
done <badruns_all_target.txt
#done <badruns_LD2_target.txt

cat badruns_all_targetv2.txt | wc -l
#cat badruns_LD2_targetv2.txt |wc -l
#cat badruns_all_targetv2.txt | xargs rm -rf 
#cat badruns_LD2_targetv2.txt | xargs rm -rf 
