var postalCodeByCity = [];

postalCodeByCity["COLOMBES"]="92700";
postalCodeByCity["HOUILLES"]="78800";
postalCodeByCity["PUTEAUX"]="92800";
postalCodeByCity["NANTERRE"]="92000";

function getPostalCode(cityname)
{
  return postalCodeByCity[cityname.toUpperCase()];
}
