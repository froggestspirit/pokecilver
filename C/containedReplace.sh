while read func; do 
    sed_script+="s/CALL(m$func)/CCALL(a$func)/g;"
done < ../containedFuncs.txt

find . -type f -name \*.c -exec sed -i "$sed_script" '{}' +
