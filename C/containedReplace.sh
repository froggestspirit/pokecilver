CFUNCS=../containedFuncs.txt

sed_script=""
while read func; do 
    sed_script+="s/CALL *(m$func)/CCALL(a$func)/g;"
done < $CFUNCS

find . -type f -name \*.c -exec sed -i "$sed_script" '{}' +

sed_script=""
while read func; do 
    sed_script+="s/CALL_NZ *(m$func)/CCALL_NZ(a$func)/g;"
done < $CFUNCS

find . -type f -name \*.c -exec sed -i "$sed_script" '{}' +

sed_script=""
while read func; do 
    sed_script+="s/CALL_Z *(m$func)/CCALL_Z(a$func)/g;"
done < $CFUNCS

find . -type f -name \*.c -exec sed -i "$sed_script" '{}' +

sed_script=""
while read func; do 
    sed_script+="s/CALL_NC *(m$func)/CCALL_NC(a$func)/g;"
done < $CFUNCS

find . -type f -name \*.c -exec sed -i "$sed_script" '{}' +

sed_script=""
while read func; do 
    sed_script+="s/CALL_C *(m$func)/CCALL_C(a$func)/g;"
done < $CFUNCS

find . -type f -name \*.c -exec sed -i "$sed_script" '{}' +

sed_script=""
while read func; do 
    sed_script+="s/JP *(m$func)/return $func()/g;"
done < $CFUNCS

find . -type f -name \*.c -exec sed -i "$sed_script" '{}' +

sed_script=""
while read func; do 
    sed_script+="s/JR *(m$func)/return $func()/g;"
done < $CFUNCS

find . -type f -name \*.c -exec sed -i "$sed_script" '{}' +

sed_script=""
while read func; do 
    sed_script+="s/JR_NZ *(m$func)/IF_NZ return $func()/g;"
done < $CFUNCS

find . -type f -name \*.c -exec sed -i "$sed_script" '{}' +

sed_script=""
while read func; do 
    sed_script+="s/JR_Z *(m$func)/IF_Z return $func()/g;"
done < $CFUNCS

find . -type f -name \*.c -exec sed -i "$sed_script" '{}' +

sed_script=""
while read func; do 
    sed_script+="s/JR_NC *(m$func)/IF_NC return $func()/g;"
done < $CFUNCS

find . -type f -name \*.c -exec sed -i "$sed_script" '{}' +

sed_script=""
while read func; do 
    sed_script+="s/JR_C *(m$func)/IF_C return $func()/g;"
done < $CFUNCS

find . -type f -name \*.c -exec sed -i "$sed_script" '{}' +
