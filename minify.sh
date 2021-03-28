#!/bin/bash

# Author: Stephane Fedim
# Description: Script to minify all css and js files before deployment
# Tools: uglify-js & cssnano


cd web/

# Create temporary folder
mkdir tmp_minify/

cp *.css tmp_minify
cp *.js tmp_minify

cd tmp_minify/

IFS='.'
# Minify all *.js files

for jsfile in *.js
do
	read -ra ADDR <<< "$jsfile"
	uglifyjs --compress --mangle --verbose --webkit --output "${ADDR[0]}.min.js" -- "$jsfile" && cp "${ADDR[0]}.min.js" /var/www/html
done
IFS=' '

IFS='.'
# Minify all *.css files
for cssfile in *.css
do
	read -ra ADDR <<< "$cssfile"
	postcss "$cssfile" > "${ADDR[0]}.min.css" && cp "${ADDR[0]}.min.css" /var/www/html
done
IFS=' '

# Go back to web folder
cd ../
# Delete tmp folder
rm -R tmp_minify/

# Enter in web server folder
# cd /var/www/html

IFS='.'
# Replace all *.js files in /var/www/html/*.html files by *.min.js
for file in /var/www/html/*.html
do
        for jsfile in web/*.js
        do
                if grep "$jsfile" "$file"
                then
                        read -ra ADDR <<< "$jsfile"
                        sed -i "s/$jsfile/${ADDR[0]}.min.js/g" "$file"
                fi
        done
done
IFS=' '

IFS='.'
# Replace all *.css files in /var/www/html*.html files by *.min.css
for file in /var/www/html/*.html
do
        for cssfile in web/*.css
        do
                if grep "$cssfile" "$file"
                then
                        read -ra ADDR <<< "$cssfile"
                        sed -i "s/$cssfile/${ADDR[0]}.min.css/g" "$file"
                fi
        done
done
IFS=' '
