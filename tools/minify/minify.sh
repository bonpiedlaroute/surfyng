#!/bin/bash

# Author: Stephane Fedim
# Description: Script to minify all css and js files before deployment
# Tools: uglify-js & cssnano

# Create temporary folder
mkdir -p tmp_minify/


cp /home/ubuntu/software/surfyng/web/*.css tmp_minify
cp /home/ubuntu/software/surfyng/web/*.js tmp_minify


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
rm -rf tmp_minify/

# Enter in web server folder
# cd /var/www/html

IFS='.'
# Replace all *.js files in /var/www/html/*.html files by *.min.js
for file in /var/www/html/*.html
do
        echo "looking for $file replacement"
        cd /home/ubuntu/software/surfyng/web/
        for jsfile in *.js
        do
                grep "$jsfile" "$file"
                if [ $? -eq 0 ]; then
                        echo "replacing $jsile in $file"
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
        echo "looking for $file replacement"
        cd /home/ubuntu/software/surfyng/web/
        for cssfile in *.css
        do
                grep "$cssfile" "$file"
                if [ $? -eq 0 ];then
                        echo "replacing $cssfile in $file"
                        read -ra ADDR <<< "$cssfile"
                        sed -i "s/$cssfile/${ADDR[0]}.min.css/g" "$file"
                fi
        done
done
IFS=' '
