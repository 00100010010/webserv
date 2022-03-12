file=$1
folder="http://localhost:8080/upload/"
path=$folder$1
rm $folder$1 2> /dev/null 

curl -X DELETE -H "Content-Type: toto" --data "" $path