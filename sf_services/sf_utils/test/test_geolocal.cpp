/*
 * Created on Mon Sep 07 2020
 *
 * Copyright (c) 2020 Surfyn
 */
#include <gtest/gtest.h>
#include "Geolocal.h"

using namespace surfyn::utils;

TEST(TestGeolocal, shouldreturnColombesInseecodeWhenCityisColombes)
{ 
   GeoLocal g("../../../../bot/data/correspondance_codeinsee_codepostal_iledefrance.csv");
   auto inseecode = g.getInseeCode("COLOMBES");

   EXPECT_EQ("92025", inseecode);
}

TEST(TestGeolocal, shouldreturnNanterreInseecodeWhenCityisNanterre)
{ 
   GeoLocal g("../../../../bot/data/correspondance_codeinsee_codepostal_iledefrance.csv");
   auto inseecode = g.getInseeCode("NANTERRE");

   EXPECT_EQ("92050", inseecode);
}

TEST(TestGeolocal, shouldreturnEmptyInseecodeWhenCityisNotFound)
{ 
   GeoLocal g("../../../../bot/data/correspondance_codeinsee_codepostal_iledefrance.csv");
   auto inseecode = g.getInseeCode("WAKANDA");

   EXPECT_EQ("", inseecode);
}

TEST(TestGeolocal, shouldreturnColombesPostalcodeWhenCityisColombes)
{ 
   GeoLocal g("../../../../bot/data/correspondance_codeinsee_codepostal_iledefrance.csv");
   auto postalcode = g.getPostalCode("COLOMBES");

   EXPECT_EQ("92700", postalcode);
}

TEST(TestGeolocal, shouldreturnNanterrePostalcodeWhenCityisNanterre)
{ 
   GeoLocal g("../../../../bot/data/correspondance_codeinsee_codepostal_iledefrance.csv");
   auto postalcode = g.getPostalCode("NANTERRE");

   EXPECT_EQ("92000", postalcode);
}

TEST(TestGeolocal, shouldreturnEmptyPostalcodeWhenCityisNotFound)
{ 
   GeoLocal g("../../../../bot/data/correspondance_codeinsee_codepostal_iledefrance.csv");
   auto postalcode = g.getPostalCode("WAKANDA");

   EXPECT_EQ("", postalcode);
}

TEST(TestGeolocal, shouldreturnColombesInseeCodeWhenPostalCodeisColombes)
{ 
   GeoLocal g("../../../../bot/data/correspondance_codeinsee_codepostal_iledefrance.csv");
   auto inseecode = g.getInseeCodeFromPostalCode("92700");

   EXPECT_EQ("92025", inseecode);
}

TEST(TestGeolocal, shouldreturnNanterreInseeCodeWhenPostalCodeisNanterre)
{ 
   GeoLocal g("../../../../bot/data/correspondance_codeinsee_codepostal_iledefrance.csv");
   auto inseecode = g.getInseeCodeFromPostalCode("92000");

   EXPECT_EQ("92050", inseecode);
}
