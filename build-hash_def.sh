#! /bin/dash

get_hash() {
  name="$1"
  ./colorforth --hash "$name"
}

print_hash() {
  name=$1
  hash_def=$2
  hash=$3
  printf "#define %-30s (hash_t) %-25s // $name\n" $hash_def $hash
}

build_reg() {
  reg=$1
  reg_op=$2
  op=$3

  name="$reg$op"
  hash_def=REG_${reg}_${reg_op}_HASH
  hash=$(get_hash "$name")
  print_hash "$name" "$hash_def" "$hash" >> $OUTFILE
}

OUTFILE=hash_def-tmp.h

# echo "Ensure last hash version"
# make

awk 'BEGIN{p=1} p==1 {print $0} /BEGIN AUTOGEN PART/ {p=0}' src/hash_def.h > $OUTFILE

for f in $(grep -rl define_primitive src/*.c ext/*.c); do
  echo "Parsing: $f"
  printf "\n// $f\n" >> $OUTFILE

  grep -r define_primitive $f | awk '/HASH/ && !/##/ {print $2" "$3}' | while read -r hash_def name; do
    hash_def=$(echo $hash_def | tr -d ',')
    name=$(echo $name | sed -e 's/ENTRY_NAME("//' -e 's/"),//g')

    hash=$(get_hash "$name")
    print_hash "$name" "$hash_def" "$hash" >> $OUTFILE
  done
done

for reg in A B C I J; do
  printf "\n// Register $reg\n" >> $OUTFILE

  build_reg $reg LOAD    "@"
  build_reg $reg STORE   "!"
  build_reg $reg ADD_STORE   "+!"
  build_reg $reg ADD_ADD "++"
  build_reg $reg SUB_SUB "--"
  build_reg $reg TO_R    ">R"

  name="R>$reg"
  hash_def=REG_R_TO_${reg}_HASH
  hash=$(get_hash "$name")
  print_hash "$name" "$hash_def" "$hash" >> $OUTFILE
done

awk 'BEGIN{p=0} /END AUTOGEN PART/ {p=1} p==1 {print $0}' src/hash_def.h >> $OUTFILE

mv $OUTFILE src/hash_def.h

echo "Rebuilding with new hash definitions"
make
