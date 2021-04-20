var postalCodeByCity = [];

postalCodeByCity["COLOMBES"]="92700";
postalCodeByCity["HOUILLES"]="78800";
postalCodeByCity["PUTEAUX"]="92800";
postalCodeByCity["NANTERRE"]="92000";

function getPostalCode(cityname)
{
  return postalCodeByCity[cityname.toUpperCase()];
}

var neighborhoodByCity = [];

neighborhoodByCity["COLOMBES"]="le quartier Plateau-Grèves-Europe-Gare du Stade-Fossés Jean, quartier Agent Sarre-Centre-Les Vallées-La Garenne,\
 quartier Gabriel Péri-Estienne d'Orves, quartier Petit Colombes-Charles de Gaulle";
neighborhoodByCity["HOUILLES"]="le quartier Belles Vues, quartier Blanches-Pierrats-Tonkin, quartier Réveil Matin, quartier Main de Fer";
neighborhoodByCity["PUTEAUX"]="le quartier Centre Ville-Vieux Puteaux, quartier République, quartier Bergère-Moulin, quartier Seine Sud, quartier Colline,\
 quartier La Défense, quartier Front de Seine-Bellini, quartier Pressensé-Rives de Seine, quartier Rosiers-Lorieux";
neighborhoodByCity["NANTERRE"]="le quartier Vieux Pont, quartier Parc-La Boule- Champs Pierreux, quartier Plateau, quartier Centre,\
 quartier République-Université, quartier Chemin de l'île, quartier Petit Nanterre, quartier Mont Valérien";

function getNeighborhoodByCity(cityname)
{
  return neighborhoodByCity[cityname.toUpperCase()];
}
