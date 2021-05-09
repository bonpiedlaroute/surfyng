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

neighborhoodByCity["COLOMBES"]="le quartier Plateau-Grèves-Europe-Gare du Stade-Fossés Jean, secteur Agent Sarre-Centre-Les Vallées-La Garenne,\
 quartier Gabriel Péri-Estienne d'Orves, secteur Petit Colombes-Charles de Gaulle";
neighborhoodByCity["HOUILLES"]="le quartier Belles Vues, secteur Blanches-Pierrats-Tonkin, quartier Réveil Matin, secteur Main de Fer";
neighborhoodByCity["PUTEAUX"]="le quartier Centre Ville-Vieux Puteaux, secteur République, quartier Bergère-Moulin, secteur Seine Sud, quartier Colline,\
 secteur La Défense, quartier Front de Seine-Bellini, secteur Pressensé-Rives de Seine, quartier Rosiers-Lorieux";
neighborhoodByCity["NANTERRE"]="le quartier Vieux Pont, secteur Parc-La Boule- Champs Pierreux, quartier Plateau, secteur Centre,\
 quartier République-Université, secteur Chemin de l'île, quartier Petit Nanterre, secteur Mont Valérien";

function getNeighborhoodByCity(cityname)
{
  return neighborhoodByCity[cityname.toUpperCase()];
}
