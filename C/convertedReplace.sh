CFUNCS=../ConvertedFunctions.txt

sed_script=""
while read func; do 
    sed_script+="s/CCALL(a$func)/$func()/g;"
done < $CFUNCS

find . -type f -name \*.c -exec sed -i "$sed_script" '{}' +

sed_script=""
while read func; do 
    sed_script+="s/CCALL_NZ(a$func)/IF_NZ $func()/g;"
done < $CFUNCS

find . -type f -name \*.c -exec sed -i "$sed_script" '{}' +

sed_script=""
while read func; do 
    sed_script+="s/CCALL_Z(a$func)/IF_Z $func()/g;"
done < $CFUNCS

find . -type f -name \*.c -exec sed -i "$sed_script" '{}' +

sed_script=""
while read func; do 
    sed_script+="s/CCALL_NC(a$func)/IF_NC $func()/g;"
done < $CFUNCS

find . -type f -name \*.c -exec sed -i "$sed_script" '{}' +

sed_script=""
while read func; do 
    sed_script+="s/CCALL_C(a$func)/IF_C $func()/g;"
done < $CFUNCS

find . -type f -name \*.c -exec sed -i "$sed_script" '{}' +
