#!/bin/bash
wget https://www.businessimmo.com/rss/logement/feed.xml -O businessimmo.xml
wget https://www.lefigaro.fr/rss/figaro_immobilier.xml -O figaro_immobilier.xml
wget "http://feed.prismamediadigital.com/v1/cap/rss?sources=capital,polemik,xerfi,management,capital-avec-agence-france-presse,capital-avec-aof,capital-avec-reuters,capital-avec-optimaretraite,aeronewstv&categories=immobilier" -O capital.xml
wget https://www.journaldelagence.com/feed -O lejournaldelagence.xml
wget https://syndication.lesechos.fr/rss/rss_immobilier_btp.xml -O lesechos_immobilier.xml
wget https://www.notaires.fr/fr/actualites-rss -O immobiliernotaires.xml
sudo mkdir -p /var/www/html/data/actus/
sudo mv *.xml /var/www/html/data/actus/
