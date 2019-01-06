/*
 * sf_classifier_test.cpp
 *
 *  Created on: 24 févr. 2018
 *      Author(s): Noel Tchidjo
 */
#include <cstdio>
#include "pch.h"
#include "RealEstateAdClassifier.h"
#include "RealEstateAd.h"
#include <fstream>

using namespace surfyn::classifier;

static void generate_simple_data()
{
   std::fstream f;
   f.open("simple_data", std::ios::out);

   const char* s_data = "[\n\
{\n\
\"ID\":\"1\",\n\
\"PRICE\":\"99045\",\n\
\"SURFACE\":\"19.61\",\n\
\"CONSTRUCTION_YEAR\":\"1985\",\n\
\"TIME_TO_PUBLICTRANSPORT\":\"7mn\",\n\
\"BEDS\":\"1\",\n\
\"CITY\":\"colombes\",\n\
\"PROPERTY_TYPE\":\"studio\",\n\
\"ROOMS\":\"1\",\n\
\"TYPE_OF_HEATING\":\"electrique\",\n\
\"CELLARS\":\"0\"\n\
}\n\
,\n\
{\n\
\"ID\":\"2\",\n\
\"PRICE\":\"99045\",\n\
\"SURFACE\":\"19.61\",\n\
\"CONSTRUCTION_YEAR\":\"1985\",\n\
\"TIME_TO_PUBLICTRANSPORT\":\"7mn\",\n\
\"BEDS\":\"1\",\n\
\"CITY\":\"colombes\",\n\
\"PROPERTY_TYPE\":\"studio\",\n\
\"ROOMS\":\"1\",\n\
\"TYPE_OF_HEATING\":\"electrique\",\n\
\"CELLARS\":\"0\"\n\
}\n\
,\n\
{\n\
\"ID\":\"3\",\n\
\"PRICE\":\"100000\",\n\
\"SURFACE\":\"15\",\n\
\"CONSTRUCTION_YEAR\":\"1960\",\n\
\"TIME_TO_PUBLICTRANSPORT\":\"2mn\",\n\
\"BEDS\":\"2\",\n\
\"CITY\":\"colombes\",\n\
\"PROPERTY_TYPE\":\"studio\",\n\
\"ROOMS\":\"3\",\n\
\"TYPE_OF_HEATING\":\"electrique\",\n\
\"CELLARS\":\"0\"\n\
}\n\
,\n\
{\n\
\"ID\":\"4\",\n\
\"PRICE\":\"100000\",\n\
\"SURFACE\":\"25\",\n\
\"CONSTRUCTION_YEAR\":\"1948\",\n\
\"TIME_TO_PUBLICTRANSPORT\":\"5mn\",\n\
\"BEDS\":\"1\",\n\
\"CITY\":\"colombes\",\n\
\"PROPERTY_TYPE\":\"appartement\",\n\
\"ROOMS\":\"2\",\n\
\"TYPE_OF_HEATING\":\"gaz\",\n\
\"CELLARS\":\"0\"\n\
}\n\
,\n\
{\n\
\"ID\":\"5\",\n\
\"PRICE\":\"99045\",\n\
\"SURFACE\":\"25\",\n\
\"CONSTRUCTION_YEAR\":\"1950\",\n\
\"TIME_TO_PUBLICTRANSPORT\":\"10mn\",\n\
\"BEDS\":\"1\",\n\
\"CITY\":\"colombes\",\n\
\"PROPERTY_TYPE\":\"appartement\",\n\
\"ROOMS\":\"3\",\n\
\"TYPE_OF_HEATING\":\"gaz\",\n\
\"CELLARS\":\"0\"\n\
}\n\
]\n";

   f << s_data;
   f.close();

   f.open("simple_similar", std::ios::out);
   f << "1;2\n";
   f.close();

}
static void generate_easy_data()
{
   std::fstream f;
   f.open("easy_data", std::ios::out);
   const char* data= "[\n\
{\n\
\"ID\":\"1\",\n\
\"PRICE\":\"99045\",\n\
\"SURFACE\":\"19.61\",\n\
\"CONSTRUCTION_YEAR\":\"\",\n\
\"TIME_TO_PUBLICTRANSPORT\":\"7mn\",\n\
\"BEDS\":\"1\",\n\
\"CITY\":\"colombes\",\n\
\"PROPERTY_TYPE\":\"appartement\",\n\
\"ROOMS\":\"1\",\n\
\"TYPE_OF_HEATING\":\"electrique\",\n\
\"CELLARS\":\"0\",\n\
\"FLOOR\":\"0\",\n\
\"PARKING\":\"\"\n\
},\n\
{\n\
\"ID\":\"2\",\n\
\"PRICE\":\"99045\",\n\
\"SURFACE\":\"19.61\",\n\
\"CONSTRUCTION_YEAR\":\"\",\n\
\"TIME_TO_PUBLICTRANSPORT\":\"7mn\",\n\
\"BEDS\":\"1\",\n\
\"CITY\":\"colombes\",\n\
\"PROPERTY_TYPE\":\"appartement\",\n\
\"ROOMS\":\"1\",\n\
\"TYPE_OF_HEATING\":\"electrique\",\n\
\"CELLARS\":\"0\",\n\
\"FLOOR\":\"\",\n\
\"PARKING\":\"\"\n\
},\n\
{\n\
\"ID\":\"3\",\n\
\"PRICE\":\"110000\",\n\
\"SURFACE\":\"18\",\n\
\"CONSTRUCTION_YEAR\":\"\",\n\
\"TIME_TO_PUBLICTRANSPORT\":\"1930\",\n\
\"BEDS\":\"1\",\n\
\"CITY\":\"\",\n\
\"PROPERTY_TYPE\":\"Appartement\",\n\
\"ROOMS\":\"1\",\n\
\"TYPE_OF_HEATING\":\"gaz\",\n\
\"CELLARS\":\"1\",\n\
\"FLOOR\":\"5\",\n\
\"PARKING\":\"\"\n\
},\n\
{\n\
\"ID\":\"4\",\n\
\"PRICE\":\"113000\",\n\
\"SURFACE\":\"18\",\n\
\"CONSTRUCTION_YEAR\":\"1930\",\n\
\"TIME_TO_PUBLICTRANSPORT\":\"\",\n\
\"BEDS\":\"1\",\n\
\"CITY\":\"colombes\",\n\
\"PROPERTY_TYPE\":\"appartement\",\n\
\"ROOMS\":\"1\",\n\
\"TYPE_OF_HEATING\":\"gaz\",\n\
\"CELLARS\":\"\",\n\
\"FLOOR\":\"5\",\n\
\"PARKING\":\"\"\n\
},\n\
{\n\
\"ID\":\"5\",\n\
\"PRICE\":\"116000\",\n\
\"SURFACE\":\"21.6\",\n\
\"CONSTRUCTION_YEAR\":\"\",\n\
\"TIME_TO_PUBLICTRANSPORT\":\"\",\n\
\"BEDS\":\"1\",\n\
\"CITY\":\"colombes\",\n\
\"PROPERTY_TYPE\":\"appartement\",\n\
\"ROOMS\":\"1\",\n\
\"TYPE_OF_HEATING\":\"electrique\",\n\
\"CELLARS\":\"1\",\n\
\"FLOOR\":\"0\",\n\
\"PARKING\":\"\"\n\
},\n\
{\n\
\"ID\":\"6\",\n\
\"PRICE\":\"119000\",\n\
\"SURFACE\":\"47\",\n\
\"CONSTRUCTION_YEAR\":\"1970\",\n\
\"TIME_TO_PUBLICTRANSPORT\":\"\",\n\
\"BEDS\":\"1\",\n\
\"CITY\":\"colombes\",\n\
\"PROPERTY_TYPE\":\"appartement\",\n\
\"ROOMS\":\"2\",\n\
\"TYPE_OF_HEATING\":\"gaz\",\n\
\"CELLARS\":\"1\",\n\
\"FLOOR\":\"2\",\n\
\"PARKING\":\"\"\n\
},\n\
{\n\
\"ID\":\"7\",\n\
\"PRICE\":\"121000\",\n\
\"SURFACE\":\"18\",\n\
\"CONSTRUCTION_YEAR\":\"\",\n\
\"TIME_TO_PUBLICTRANSPORT\":\"6mn\",\n\
\"BEDS\":\"1\",\n\
\"CITY\":\"colombes\",\n\
\"PROPERTY_TYPE\":\"appartement\",\n\
\"ROOMS\":\"1\",\n\
\"TYPE_OF_HEATING\":\"gaz\",\n\
\"CELLARS\":\"1\",\n\
\"FLOOR\":\"5\",\n\
\"PARKING\":\"\"\n\
},\n\
{\n\
\"ID\":\"8\",\n\
\"PRICE\":\"122475\",\n\
\"SURFACE\":\"18\",\n\
\"CONSTRUCTION_YEAR\":\"\",\n\
\"TIME_TO_PUBLICTRANSPORT\":\"\",\n\
\"BEDS\":\"1\",\n\
\"CITY\":\"colombes\",\n\
\"PROPERTY_TYPE\":\"appartement\",\n\
\"ROOMS\":\"1\",\n\
\"TYPE_OF_HEATING\":\"gaz\",\n\
\"CELLARS\":\"1\",\n\
\"FLOOR\":\"5\",\n\
\"PARKING\":\"\"\n\
},\n\
{\n\
\"ID\":\"9\",\n\
\"PRICE\":\"122475\",\n\
\"SURFACE\":\"18\",\n\
\"CONSTRUCTION_YEAR\":\"\",\n\
\"TIME_TO_PUBLICTRANSPORT\":\"\",\n\
\"BEDS\":\"1\",\n\
\"CITY\":\"colombes\",\n\
\"PROPERTY_TYPE\":\"appartement\",\n\
\"ROOMS\":\"1\",\n\
\"TYPE_OF_HEATING\":\"gaz\",\n\
\"CELLARS\":\"1\",\n\
\"FLOOR\":\"5\",\n\
\"PARKING\":\"\"\n\
},\n\
{\n\
\"ID\":\"10\",\n\
\"PRICE\":\"123625\",\n\
\"SURFACE\":\"28\",\n\
\"CONSTRUCTION_YEAR\":\"\",\n\
\"TIME_TO_PUBLICTRANSPORT\":\"7mn\",\n\
\"BEDS\":\"1\",\n\
\"CITY\":\"colombes\",\n\
\"PROPERTY_TYPE\":\"appartement\",\n\
\"ROOMS\":\"1\",\n\
\"TYPE_OF_HEATING\":\"\",\n\
\"CELLARS\":\"\",\n\
\"FLOOR\":\"0\",\n\
\"PARKING\":\"\"\n\
},\n\
{\n\
\"ID\":\"11\",\n\
\"PRICE\":\"124000\",\n\
\"SURFACE\":\"24.96\",\n\
\"CONSTRUCTION_YEAR\":\"1980\",\n\
\"TIME_TO_PUBLICTRANSPORT\":\"\",\n\
\"BEDS\":\"1\",\n\
\"CITY\":\"colombes\",\n\
\"PROPERTY_TYPE\":\"appartement\",\n\
\"ROOMS\":\"1\",\n\
\"TYPE_OF_HEATING\":\"\",\n\
\"CELLARS\":\"1\",\n\
\"FLOOR\":\"4\",\n\
\"PARKING\":\"\"\n\
},\n\
{\n\
\"ID\":\"12\",\n\
\"PRICE\":\"124000\",\n\
\"SURFACE\":\"25\",\n\
\"CONSTRUCTION_YEAR\":\"1980\",\n\
\"TIME_TO_PUBLICTRANSPORT\":\"\",\n\
\"BEDS\":\"1\",\n\
\"CITY\":\"clombes\",\n\
\"PROPERTY_TYPE\":\"appartement\",\n\
\"ROOMS\":\"1\",\n\
\"TYPE_OF_HEATING\":\"\",\n\
\"CELLARS\":\"1\",\n\
\"FLOOR\":\"4\",\n\
\"PARKING\":\"\"\n\
},\n\
{\n\
\"ID\":\"13\",\n\
\"PRICE\":\"127000\",\n\
\"SURFACE\":\"21.28\",\n\
\"CONSTRUCTION_YEAR\":\"1949\",\n\
\"TIME_TO_PUBLICTRANSPORT\":\"\",\n\
\"BEDS\":\"1\",\n\
\"CITY\":\"colombes\",\n\
\"PROPERTY_TYPE\":\"appartement\",\n\
\"ROOMS\":\"2\",\n\
\"TYPE_OF_HEATING\":\"gaz\",\n\
\"CELLARS\":\"1\",\n\
\"FLOOR\":\"\",\n\
\"PARKING\":\"\"\n\
},\n\
{\n\
\"ID\":\"14\",\n\
\"PRICE\":\"128000\",\n\
\"SURFACE\":\"25\",\n\
\"CONSTRUCTION_YEAR\":\"1930\",\n\
\"TIME_TO_PUBLICTRANSPORT\":\"\",\n\
\"BEDS\":\"1\",\n\
\"CITY\":\"\",\n\
\"PROPERTY_TYPE\":\"appartement\",\n\
\"ROOMS\":\"1\",\n\
\"TYPE_OF_HEATING\":\"gaz\",\n\
\"CELLARS\":\"1\",\n\
\"FLOOR\":\"2\",\n\
\"PARKING\":\"\"\n\
},\n\
{\n\
\"ID\":\"15\",\n\
\"PRICE\":\"128000\",\n\
\"SURFACE\":\"23\",\n\
\"CONSTRUCTION_YEAR\":\"1930\",\n\
\"TIME_TO_PUBLICTRANSPORT\":\"\",\n\
\"BEDS\":\"1\",\n\
\"CITY\":\"colombes\",\n\
\"PROPERTY_TYPE\":\"appartement\",\n\
\"ROOMS\":\"1\",\n\
\"TYPE_OF_HEATING\":\"electrique\",\n\
\"CELLARS\":\"\",\n\
\"FLOOR\":\"6\",\n\
\"PARKING\":\"\"\n\
},\n\
{\n\
\"ID\":\"16\",\n\
\"PRICE\":\"130000\",\n\
\"SURFACE\":\"24\",\n\
\"CONSTRUCTION_YEAR\":\"1930\",\n\
\"TIME_TO_PUBLICTRANSPORT\":\"5mn\",\n\
\"BEDS\":\"1\",\n\
\"CITY\":\"colombes\",\n\
\"PROPERTY_TYPE\":\"appartement\",\n\
\"ROOMS\":\"1\",\n\
\"TYPE_OF_HEATING\":\"gaz\",\n\
\"CELLARS\":\"1\",\n\
\"FLOOR\":\"1\",\n\
\"PARKING\":\"\"\n\
},\n\
{\n\
\"ID\":\"17\",\n\
\"PRICE\":\"130000\",\n\
\"SURFACE\":\"24\",\n\
\"CONSTRUCTION_YEAR\":\"\",\n\
\"TIME_TO_PUBLICTRANSPORT\":\"5mn\",\n\
\"BEDS\":\"1\",\n\
\"CITY\":\"colombes\",\n\
\"PROPERTY_TYPE\":\"appartement\",\n\
\"ROOMS\":\"1\",\n\
\"TYPE_OF_HEATING\":\"gaz\",\n\
\"CELLARS\":\"1\",\n\
\"FLOOR\":\"1\",\n\
\"PARKING\":\"\"\n\
},\n\
{\n\
\"ID\":\"18\",\n\
\"PRICE\":\"135000\",\n\
\"SURFACE\":\"24\",\n\
\"CONSTRUCTION_YEAR\":\"\",\n\
\"TIME_TO_PUBLICTRANSPORT\":\"5mn\",\n\
\"BEDS\":\"1\",\n\
\"CITY\":\"colombes\",\n\
\"PROPERTY_TYPE\":\"appartement\",\n\
\"ROOMS\":\"1\",\n\
\"TYPE_OF_HEATING\":\"gaz\",\n\
\"CELLARS\":\"1\",\n\
\"FLOOR\":\"1\",\n\
\"PARKING\":\"\"\n\
},\n\
{\n\
\"ID\":\"19\",\n\
\"PRICE\":\"135000\",\n\
\"SURFACE\":\"24\",\n\
\"CONSTRUCTION_YEAR\":\"5mn\",\n\
\"TIME_TO_PUBLICTRANSPORT\":\"\",\n\
\"BEDS\":\"1\",\n\
\"CITY\":\"colombes\",\n\
\"PROPERTY_TYPE\":\"appartement\",\n\
\"ROOMS\":\"1\",\n\
\"TYPE_OF_HEATING\":\"gaz\",\n\
\"CELLARS\":\"1\",\n\
\"FLOOR\":\"1\",\n\
\"PARKING\":\"\"\n\
},\n\
{\n\
\"ID\":\"20\",\n\
\"PRICE\":\"136000\",\n\
\"SURFACE\":\"31\",\n\
\"CONSTRUCTION_YEAR\":\"1970\",\n\
\"TIME_TO_PUBLICTRANSPORT\":\"\",\n\
\"BEDS\":\"1\",\n\
\"CITY\":\"colombes\",\n\
\"PROPERTY_TYPE\":\"appartement\",\n\
\"ROOMS\":\"1\",\n\
\"TYPE_OF_HEATING\":\"gaz\",\n\
\"CELLARS\":\"1\",\n\
\"FLOOR\":\"2\",\n\
\"PARKING\":\"1\"\n\
},\n\
{\n\
\"ID\":\"21\",\n\
\"PRICE\":\"138000\",\n\
\"SURFACE\":\"23\",\n\
\"CONSTRUCTION_YEAR\":\"\",\n\
\"TIME_TO_PUBLICTRANSPORT\":\"\",\n\
\"BEDS\":\"1\",\n\
\"CITY\":\"colombes\",\n\
\"PROPERTY_TYPE\":\"appartement\",\n\
\"ROOMS\":\"1\",\n\
\"TYPE_OF_HEATING\":\"\",\n\
\"CELLARS\":\"\",\n\
\"FLOOR\":\"0\",\n\
\"PARKING\":\"\"\n\
},\n\
{\n\
\"ID\":\"22\",\n\
\"PRICE\":\"138000\",\n\
\"SURFACE\":\"23\",\n\
\"CONSTRUCTION_YEAR\":\"\",\n\
\"TIME_TO_PUBLICTRANSPORT\":\"\",\n\
\"BEDS\":\"1\",\n\
\"CITY\":\"colombes\",\n\
\"PROPERTY_TYPE\":\"appartement\",\n\
\"ROOMS\":\"1\",\n\
\"TYPE_OF_HEATING\":\"\",\n\
\"CELLARS\":\"\",\n\
\"FLOOR\":\"\",\n\
\"PARKING\":\"\"\n\
},\n\
{\n\
\"ID\":\"23\",\n\
\"PRICE\":\"139000\",\n\
\"SURFACE\":\"34\",\n\
\"CONSTRUCTION_YEAR\":\"1930\",\n\
\"TIME_TO_PUBLICTRANSPORT\":\"\",\n\
\"BEDS\":\"1\",\n\
\"CITY\":\"colombes\",\n\
\"PROPERTY_TYPE\":\"appartement\",\n\
\"ROOMS\":\"1\",\n\
\"TYPE_OF_HEATING\":\"gaz\",\n\
\"CELLARS\":\"1\",\n\
\"FLOOR\":\"\",\n\
\"PARKING\":\"\"\n\
},\n\
{\n\
\"ID\":\"24\",\n\
\"PRICE\":\"139000\",\n\
\"SURFACE\":\"34\",\n\
\"CONSTRUCTION_YEAR\":\"\",\n\
\"TIME_TO_PUBLICTRANSPORT\":\"\",\n\
\"BEDS\":\"1\",\n\
\"CITY\":\"colombes\",\n\
\"PROPERTY_TYPE\":\"appartement\",\n\
\"ROOMS\":\"1\",\n\
\"TYPE_OF_HEATING\":\"\",\n\
\"CELLARS\":\"1\",\n\
\"FLOOR\":\"\",\n\
\"PARKING\":\"\"\n\
},\n\
{\n\
\"ID\":\"25\",\n\
\"PRICE\":\"140000\",\n\
\"SURFACE\":\"30\",\n\
\"CONSTRUCTION_YEAR\":\"\",\n\
\"TIME_TO_PUBLICTRANSPORT\":\"\",\n\
\"BEDS\":\"1\",\n\
\"CITY\":\"colombes\",\n\
\"PROPERTY_TYPE\":\"appartement\",\n\
\"ROOMS\":\"2\",\n\
\"TYPE_OF_HEATING\":\"gaz\",\n\
\"CELLARS\":\"1\",\n\
\"FLOOR\":\"3\",\n\
\"PARKING\":\"\"\n\
},\n\
{\n\
\"ID\":\"26\",\n\
\"PRICE\":\"140000\",\n\
\"SURFACE\":\"29.97\",\n\
\"CONSTRUCTION_YEAR\":\"\",\n\
\"TIME_TO_PUBLICTRANSPORT\":\"\",\n\
\"BEDS\":\"1\",\n\
\"CITY\":\"colombes\",\n\
\"PROPERTY_TYPE\":\"appartement\",\n\
\"ROOMS\":\"2\",\n\
\"TYPE_OF_HEATING\":\"gaz\",\n\
\"CELLARS\":\"1\",\n\
\"FLOOR\":\"3\",\n\
\"PARKING\":\"\"\n\
},\n\
{\n\
\"ID\":\"27\",\n\
\"PRICE\":\"146000\",\n\
\"SURFACE\":\"21.59\",\n\
\"CONSTRUCTION_YEAR\":\"2012\",\n\
\"TIME_TO_PUBLICTRANSPORT\":\"1mn\",\n\
\"BEDS\":\"1\",\n\
\"CITY\":\"colombes\",\n\
\"PROPERTY_TYPE\":\"appartement\",\n\
\"ROOMS\":\"1\",\n\
\"TYPE_OF_HEATING\":\"\",\n\
\"CELLARS\":\"\",\n\
\"FLOOR\":\"2\",\n\
\"PARKING\":\"\"\n\
},\n\
{\n\
\"ID\":\"28\",\n\
\"PRICE\":\"146000\",\n\
\"SURFACE\":\"21.59\",\n\
\"CONSTRUCTION_YEAR\":\"\",\n\
\"TIME_TO_PUBLICTRANSPORT\":\"1mn\",\n\
\"BEDS\":\"1\",\n\
\"CITY\":\"colombes\",\n\
\"PROPERTY_TYPE\":\"appartement\",\n\
\"ROOMS\":\"1\",\n\
\"TYPE_OF_HEATING\":\"\",\n\
\"CELLARS\":\"\",\n\
\"FLOOR\":\"2\",\n\
\"PARKING\":\"\"\n\
},\n\
{\n\
\"ID\":\"29\",\n\
\"PRICE\":\"146000\",\n\
\"SURFACE\":\"32\",\n\
\"CONSTRUCTION_YEAR\":\"1924\",\n\
\"TIME_TO_PUBLICTRANSPORT\":\"\",\n\
\"BEDS\":\"1\",\n\
\"CITY\":\"colombes\",\n\
\"PROPERTY_TYPE\":\"appartement\",\n\
\"ROOMS\":\"2\",\n\
\"TYPE_OF_HEATING\":\"gaz\",\n\
\"CELLARS\":\"1\",\n\
\"FLOOR\":\"5\",\n\
\"PARKING\":\"\"\n\
},\n\
{\n\
\"ID\":\"30\",\n\
\"PRICE\":\"147000\",\n\
\"SURFACE\":\"24\",\n\
\"CONSTRUCTION_YEAR\":\"1930\",\n\
\"TIME_TO_PUBLICTRANSPORT\":\"\",\n\
\"BEDS\":\"1\",\n\
\"CITY\":\"colombes\",\n\
\"PROPERTY_TYPE\":\"appartement\",\n\
\"ROOMS\":\"1\",\n\
\"TYPE_OF_HEATING\":\"gaz\",\n\
\"CELLARS\":\"\",\n\
\"FLOOR\":\"3\",\n\
\"PARKING\":\"\"\n\
},\n\
{\n\
\"ID\":\"31\",\n\
\"PRICE\":\"155000\",\n\
\"SURFACE\":\"22.45\",\n\
\"CONSTRUCTION_YEAR\":\"2012\",\n\
\"TIME_TO_PUBLICTRANSPORT\":\"1mn\",\n\
\"BEDS\":\"1\",\n\
\"CITY\":\"colombes\",\n\
\"PROPERTY_TYPE\":\"appartement\",\n\
\"ROOMS\":\"1\",\n\
\"TYPE_OF_HEATING\":\"\",\n\
\"CELLARS\":\"\",\n\
\"FLOOR\":\"\",\n\
\"PARKING\":\"\"\n\
},\n\
{\n\
\"ID\":\"32\",\n\
\"PRICE\":\"155000\",\n\
\"SURFACE\":\"22.45\",\n\
\"CONSTRUCTION_YEAR\":\"\",\n\
\"TIME_TO_PUBLICTRANSPORT\":\"1mn\",\n\
\"BEDS\":\"1\",\n\
\"CITY\":\"colombes\",\n\
\"PROPERTY_TYPE\":\"appartement\",\n\
\"ROOMS\":\"1\",\n\
\"TYPE_OF_HEATING\":\"\",\n\
\"CELLARS\":\"\",\n\
\"FLOOR\":\"\",\n\
\"PARKING\":\"\"\n\
},\n\
{\n\
\"ID\":\"33\",\n\
\"PRICE\":\"156000\",\n\
\"SURFACE\":\"69\",\n\
\"CONSTRUCTION_YEAR\":\"\",\n\
\"TIME_TO_PUBLICTRANSPORT\":\"10mn\",\n\
\"BEDS\":\"2\",\n\
\"CITY\":\"colombes\",\n\
\"PROPERTY_TYPE\":\"appartement\",\n\
\"ROOMS\":\"3\",\n\
\"TYPE_OF_HEATING\":\"gaz\",\n\
\"CELLARS\":\"\",\n\
\"FLOOR\":\"16\",\n\
\"PARKING\":\"1\"\n\
},\n\
{\n\
\"ID\":\"34\",\n\
\"PRICE\":\"156000\",\n\
\"SURFACE\":\"69\",\n\
\"CONSTRUCTION_YEAR\":\"\",\n\
\"TIME_TO_PUBLICTRANSPORT\":\"10mn\",\n\
\"BEDS\":\"2\",\n\
\"CITY\":\"colombes\",\n\
\"PROPERTY_TYPE\":\"appartement\",\n\
\"ROOMS\":\"3\",\n\
\"TYPE_OF_HEATING\":\"gaz\",\n\
\"CELLARS\":\"\",\n\
\"FLOOR\":\"16\",\n\
\"PARKING\":\"1\"\n\
},\n\
{\n\
\"ID\":\"35\",\n\
\"PRICE\":\"157500\",\n\
\"SURFACE\":\"27\",\n\
\"CONSTRUCTION_YEAR\":\"1930\",\n\
\"TIME_TO_PUBLICTRANSPORT\":\"\",\n\
\"BEDS\":\"1\",\n\
\"CITY\":\"colombes\",\n\
\"PROPERTY_TYPE\":\"appartement\",\n\
\"ROOMS\":\"1\",\n\
\"TYPE_OF_HEATING\":\"gaz\",\n\
\"CELLARS\":\"1\",\n\
\"FLOOR\":\"3\",\n\
\"PARKING\":\"\"\n\
},\n\
{\n\
\"ID\":\"36\",\n\
\"PRICE\":\"157500\",\n\
\"SURFACE\":\"27\",\n\
\"CONSTRUCTION_YEAR\":\"\",\n\
\"TIME_TO_PUBLICTRANSPORT\":\"\",\n\
\"BEDS\":\"1\",\n\
\"CITY\":\"colombes\",\n\
\"PROPERTY_TYPE\":\"appartement\",\n\
\"ROOMS\":\"1\",\n\
\"TYPE_OF_HEATING\":\"\",\n\
\"CELLARS\":\"1\",\n\
\"FLOOR\":\"3\",\n\
\"PARKING\":\"\"\n\
},\n\
{\n\
\"ID\":\"37\",\n\
\"PRICE\":\"159000\",\n\
\"SURFACE\":\"62\",\n\
\"CONSTRUCTION_YEAR\":\"\",\n\
\"TIME_TO_PUBLICTRANSPORT\":\"\",\n\
\"BEDS\":\"2\",\n\
\"CITY\":\"colombes\",\n\
\"PROPERTY_TYPE\":\"appartement\",\n\
\"ROOMS\":\"3\",\n\
\"TYPE_OF_HEATING\":\"gaz\",\n\
\"CELLARS\":\"1\",\n\
\"FLOOR\":\"12\",\n\
\"PARKING\":\"1\"\n\
},\n\
{\n\
\"ID\":\"38\",\n\
\"PRICE\":\"159000\",\n\
\"SURFACE\":\"62\",\n\
\"CONSTRUCTION_YEAR\":\"\",\n\
\"TIME_TO_PUBLICTRANSPORT\":\"\",\n\
\"BEDS\":\"2\",\n\
\"CITY\":\"colombes\",\n\
\"PROPERTY_TYPE\":\"appartement\",\n\
\"ROOMS\":\"3\",\n\
\"TYPE_OF_HEATING\":\"gaz\",\n\
\"CELLARS\":\"1\",\n\
\"FLOOR\":\"12\",\n\
\"PARKING\":\"1\"\n\
},\n\
{\n\
\"ID\":\"39\",\n\
\"PRICE\":\"159000\",\n\
\"SURFACE\":\"62\",\n\
\"CONSTRUCTION_YEAR\":\"\",\n\
\"TIME_TO_PUBLICTRANSPORT\":\"\",\n\
\"BEDS\":\"2\",\n\
\"CITY\":\"colombes\",\n\
\"PROPERTY_TYPE\":\"appartement\",\n\
\"ROOMS\":\"3\",\n\
\"TYPE_OF_HEATING\":\"gaz\",\n\
\"CELLARS\":\"1\",\n\
\"FLOOR\":\"12\",\n\
\"PARKING\":\"\"\n\
},\n\
{\n\
\"ID\":\"40\",\n\
\"PRICE\":\"160000\",\n\
\"SURFACE\":\"70.46\",\n\
\"CONSTRUCTION_YEAR\":\"\",\n\
\"TIME_TO_PUBLICTRANSPORT\":\"\",\n\
\"BEDS\":\"2\",\n\
\"CITY\":\"colombes\",\n\
\"PROPERTY_TYPE\":\"appartement\",\n\
\"ROOMS\":\"3\",\n\
\"TYPE_OF_HEATING\":\"gaz\",\n\
\"CELLARS\":\"\",\n\
\"FLOOR\":\"14\",\n\
\"PARKING\":\"1\"\n\
}\n\
]\n";

   f << data;
   f.close();

   f.open("easy_similar", std::ios::out);

   f << "1;2\n\
4;7;8;9\n\
11;12\n\
16;17\n\
21;22\n\
23;24\n\
25;26\n\
27;28\n\
31;32\n\
33;34\n\
35;36\n\
37;38\n";

   f.close();

}

TEST(TestClassifierIsSame, ShouldReturnTrueWithEasyDataWhenIsSame)
{
   std::fstream f;
   f.open("sf_classifier.ini", std::ios::out);
   f << "realestate_data_file=easy_data";
   f << "\n";
   f << "similar_ad_file=easy_similar";
   f << "\n";
   f.close();

   generate_easy_data();


   RealEstateAdClassifier classifier;
   classifier.trainClassifier();
   RealEstateAd prop1(16), prop2(17);

   prop1.setDescription("PRICE","130000");
   prop1.setDescription("SURFACE","24");
   prop1.setDescription("CONSTRUCTION_YEAR","1930");
   prop1.setDescription("TIME_TO_PUBLICTRANSPORT","5mn");
   prop1.setDescription("BEDS","1");
   prop1.setDescription("CITY","colombes");
   prop1.setDescription("PROPERTY_TYPE","appartement");
   prop1.setDescription("ROOMS","1");
   prop1.setDescription("TYPE_OF_HEATING","gaz");
   prop1.setDescription("CELLARS","1");
   prop1.setDescription("FLOOR","1");
   prop1.setDescription("PARKING","");

   prop2.setDescription("PRICE","130000");
   prop2.setDescription("SURFACE","24");
   prop2.setDescription("CONSTRUCTION_YEAR","");
   prop2.setDescription("TIME_TO_PUBLICTRANSPORT","5mn");
   prop2.setDescription("BEDS","1");
   prop2.setDescription("CITY","colombes");
   prop2.setDescription("PROPERTY_TYPE","appartement");
   prop2.setDescription("ROOMS","1");
   prop2.setDescription("TYPE_OF_HEATING","gaz");
   prop2.setDescription("CELLARS","1");
   prop2.setDescription("FLOOR","1");
   prop2.setDescription("PARKING","");

   EXPECT_EQ(true, classifier.isSame( prop1, prop2));


   std::remove("sf_classifier.ini");
   std::remove("easy_data");
   std::remove("easy_similar");
}

TEST(TestClassifierIsSame, ShouldReturnTrueWithSimpleDataWhenIsSame)
{

   std::fstream f;
   f.open("sf_classifier.ini", std::ios::out);
   f << "realestate_data_file=simple_data";
   f << "\n";
   f << "similar_ad_file=simple_similar";
   f << "\n";
   f.close();

   generate_simple_data();

   RealEstateAdClassifier classifier;
   classifier.trainClassifier();
   RealEstateAd prop1(9), prop2(10);

   prop1.setDescription("PRICE","99045");
   prop1.setDescription("SURFACE","19.61");
   prop1.setDescription("ROOMS","1");


   prop2.setDescription("PRICE","99045");
   prop2.setDescription("SURFACE","19.61");
   prop2.setDescription("ROOMS","1");


   EXPECT_EQ(true, classifier.isSame( prop1, prop2));

   std::remove("sf_classifier.ini");
   std::remove("simple_data");
   std::remove("simple_similar");
}

TEST(TestClassifierIsSame, ShouldReturnFalseWithEasyDataWhenIsNotSame)
{
   std::fstream f;
   f.open("sf_classifier.ini", std::ios::out);
   f << "realestate_data_file=easy_data";
   f << "\n";
   f << "similar_ad_file=easy_similar";
   f << "\n";
   f.close();

   generate_easy_data();


   RealEstateAdClassifier classifier;
   classifier.trainClassifier();
   RealEstateAd prop1(1), prop2(2);

   prop1.setDescription("PRICE","130000");
   prop1.setDescription("SURFACE","24");
   prop1.setDescription("CONSTRUCTION_YEAR","1930");
   prop1.setDescription("TIME_TO_PUBLICTRANSPORT","5mn");
   prop1.setDescription("BEDS","1");
   prop1.setDescription("CITY","colombes");
   prop1.setDescription("PROPERTY_TYPE","appartement");
   prop1.setDescription("ROOMS","1");
   prop1.setDescription("TYPE_OF_HEATING","gaz");
   prop1.setDescription("CELLARS","1");
   prop1.setDescription("FLOOR","1");
   prop1.setDescription("PARKING","");

   prop2.setDescription("PRICE","125000");
   prop2.setDescription("SURFACE","20");
   prop2.setDescription("CONSTRUCTION_YEAR","");
   prop2.setDescription("TIME_TO_PUBLICTRANSPORT","2mn");
   prop2.setDescription("BEDS","2");
   prop2.setDescription("CITY","paris");
   prop2.setDescription("PROPERTY_TYPE","studio");
   prop2.setDescription("ROOMS","2");
   prop2.setDescription("TYPE_OF_HEATING","electrique");
   prop2.setDescription("CELLARS","");
   prop2.setDescription("FLOOR","4");
   prop2.setDescription("PARKING","");

   EXPECT_EQ(false, classifier.isSame( prop1, prop2));


   std::remove("sf_classifier.ini");
   std::remove("easy_data");
   std::remove("easy_similar");
}

TEST(TestClassifierIsSame, ShouldReturnTrueWithEasyDataWhenIsAgainTheSame)
{
   std::fstream f;
   f.open("sf_classifier.ini", std::ios::out);
   f << "realestate_data_file=easy_data";
   f << "\n";
   f << "similar_ad_file=easy_similar";
   f << "\n";
   f.close();

   generate_easy_data();


   RealEstateAdClassifier classifier;
   classifier.trainClassifier();
   RealEstateAd prop1(1), prop2(2);

   prop1.setDescription("PRICE","140000");
   prop1.setDescription("SURFACE","30");
   prop1.setDescription("CONSTRUCTION_YEAR","");
   prop1.setDescription("TIME_TO_PUBLICTRANSPORT","");
   prop1.setDescription("BEDS","1");
   prop1.setDescription("CITY","colombes");
   prop1.setDescription("PROPERTY_TYPE","appartement");
   prop1.setDescription("ROOMS","2");
   prop1.setDescription("TYPE_OF_HEATING","gaz");
   prop1.setDescription("CELLARS","1");
   prop1.setDescription("FLOOR","3");
   prop1.setDescription("PARKING","");

   prop2.setDescription("PRICE","140000");
   prop2.setDescription("SURFACE","29.97");
   prop2.setDescription("CONSTRUCTION_YEAR","");
   prop2.setDescription("TIME_TO_PUBLICTRANSPORT","2mn");
   prop2.setDescription("BEDS","1");
   prop2.setDescription("CITY","colombes");
   prop2.setDescription("PROPERTY_TYPE","appartement");
   prop2.setDescription("ROOMS","2");
   prop2.setDescription("TYPE_OF_HEATING","gaz");
   prop2.setDescription("CELLARS","1");
   prop2.setDescription("FLOOR","3");
   prop2.setDescription("PARKING","");

   EXPECT_EQ(true, classifier.isSame( prop1, prop2));


   std::remove("sf_classifier.ini");
   std::remove("easy_data");
   std::remove("easy_similar");
}

