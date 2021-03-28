cd web/

# Minify all *.js files
uglifyjs --compress --mangle --verbose --webkit --output autocomplete.min.js -- autocomplete.js && cp autocomplete.min.js /var/www/html
uglifyjs --compress --mangle --verbose --webkit --output dropdownmenu.min.js -- dropdownmenu.js && cp dropdownmenu.min.js /var/www/html
uglifyjs --compress --mangle --verbose --webkit --output email.min.js -- email.js && cp email.min.js /var/www/html
uglifyjs --compress --mangle --verbose --webkit --output estimate_house_price.min.js -- estimate_house_price.js && cp estimate_house_price.min.js /var/www/html
uglifyjs --compress --mangle --verbose --webkit --output estimate_results.min.js -- estimate_results.js && cp estimate_results.min.js /var/www/html
uglifyjs --compress --mangle --verbose --webkit --output fetch_details.min.js -- fetch_details.js && cp fetch_details.min.js /var/www/html
uglifyjs --compress --mangle --verbose --webkit --output fetch_summary.min.js -- fetch_summary.js && cp fetch_summary.min.js /var/www/html
uglifyjs --compress --mangle --verbose --webkit --output geolocal.min.js -- geolocal.js && cp geolocal.min.js /var/www/html
uglifyjs --compress --mangle --verbose --webkit --output index.min.js -- index.js && cp index.min.js /var/www/html
uglifyjs --compress --mangle --verbose --webkit --output login.min.js -- login.js && cp login.min.js /var/www/html
uglifyjs --compress --mangle --verbose --webkit --output mysearch.min.js -- mysearch.js && cp mysearch.min.js /var/www/html
uglifyjs --compress --mangle --verbose --webkit --output predicted_price.min.js -- predicted_price.js && cp predicted_price.min.js /var/www/html
uglifyjs --compress --mangle --verbose --webkit --output refresh_actu.min.js -- refresh_actu.js && cp refresh_actu.min.js /var/www/html
uglifyjs --compress --mangle --verbose --webkit --output refresh_scroll.min.js -- refresh_scroll.js && cp refresh_scroll.min.js /var/www/html
uglifyjs --compress --mangle --verbose --webkit --output surfyn.min.js -- surfyn.js &&  cp surfyn.min.js /var/www/html
uglifyjs --compress --mangle --verbose --webkit --output utils.min.js -- utils.js && cp utils.min.js /var/www/html

# Minify all *.css files
postcss autocomplete.css > autocomplete.min.css && cp autocomplete.min.css /var/www/html
postcss login.css > login.min.css && cp login.min.css /var/www/html
postcss surfyn.css > surfyn.min.css && cp surfyn.min.css /var/www/html
postcss surfyn_immo_actu.css > surfyn_immo.min.css && cp surfyn_immo.min.css /var/www/html
postcss surfyn_immo_advice.css > surfyn_immo_advice.min.css && cp surfyn_immo_advice.min.css /var/www/html


IFS='.'
for file in *.html
do
        for jsfile in *.js
        do
                if grep "$jsfile" "$file"
                then
                        read -ra ADDR <<< "$jsfile"
                        sed -i "s/$jsfile/${ADDR[0]}.min.js/g" "$file"
                fi
        done
done

IFS='.'
for file in *.html
do
        for cssfile in *.css
        do
                if grep "$cssfile" "$file"
                then
                        read -ra ADDR <<< "$cssfile"
                        sed -i "s/$cssfile/${ADDR[0]}.min.css/g" "$file"
                fi
        done
done
IFS=' '

# Set the ownership to the current user
chown -R $(id -u):$(id -g) *.min.js
chown -R $(id -u):$(id -g) *.min.css
