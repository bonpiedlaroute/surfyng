#!/usr/bin/env python
# coding: utf-8

# In[1]:


import os
import pandas as pd
import numpy as np
HOUSING_PATH = "/home/bonpiedlaroute/boulot/data/geo"
def load_housing_data(housing_path = HOUSING_PATH):
    #csv_data = os.path.join(housing_path, "valeursfoncieres-2018.txt")
    csv_data = os.path.join(housing_path, "vf_18_17_16_15_14.csv")
    #csv_data = os.path.join(housing_path, "vf_paris14.txt")
    return pd.read_csv(csv_data, sep=',', dtype={'valeur_fonciere': float}, decimal='.')
housing = load_housing_data()
housing.head()


# In[2]:


clean_housing=housing.drop(["id_mutation", "numero_disposition","adresse_suffixe","adresse_nom_voie","adresse_code_voie","code_nature_culture", "nature_culture"], axis=1)


# In[3]:


clean_housing = clean_housing.drop(["adresse_numero", "code_commune", "nom_commune","code_departement","ancien_code_commune","ancien_nom_commune", "code_nature_culture_speciale","nature_culture_speciale"], axis=1)


# In[4]:


clean_housing = clean_housing.drop(["id_parcelle","ancien_id_parcelle","id_parcelle","numero_volume","lot1_numero","lot2_numero","lot3_numero","lot4_numero","lot5_numero","type_local"], axis=1)


# In[5]:


clean_housing.head()


# In[6]:


clean_housing.dropna(subset=["nature_mutation"], inplace=True)
clean_housing = clean_housing.where(clean_housing['nature_mutation'] != "Echange")
clean_housing = clean_housing.drop(["nature_mutation"], axis=1)
clean_housing.dropna(subset=["code_type_local"], inplace=True)
clean_housing = clean_housing.where(clean_housing['code_type_local'] != 3.0)
clean_housing = clean_housing.where(clean_housing['code_type_local'] != 4.0)
clean_housing.describe()


# In[7]:


clean_housing["lot1_surface_carrez"].fillna(value=0, inplace=True)
clean_housing["lot2_surface_carrez"].fillna(value=0, inplace=True)
clean_housing["lot3_surface_carrez"].fillna(value=0, inplace=True)
clean_housing["lot4_surface_carrez"].fillna(value=0, inplace=True)
clean_housing["lot5_surface_carrez"].fillna(value=0, inplace=True)
clean_housing["surface_terrain"].fillna(value=0, inplace=True)
#clean_housing["Code type local"].fillna(value=2, inplace=True)
clean_housing.info()


# In[8]:


clean_housing.head()


# In[9]:


clean_housing = clean_housing.dropna(subset=["code_postal"])


# In[10]:


clean_housing=clean_housing.dropna(subset=["valeur_fonciere"])


# In[11]:


clean_housing.info()


# In[12]:


clean_housing.head(10)


# In[13]:


get_ipython().magic(u'matplotlib inline')
import matplotlib.pyplot as plt
clean_housing.hist(bins=50, figsize=(20,15))



# In[14]:


#clean_housing["Valeur fonciere"] = clean_housing["Valeur fonciere"] / 100Q1
#tmp_val1 = tmp_val.where(tmp_val > 25)
#tmp_val1.describe()
#clean_housing["Code type local"].where(clean_housing["Code type local"] ==3 ).describe()


# In[15]:


clean_housing.info()
clean_housing['date_mutation'] = pd.to_datetime(clean_housing['date_mutation'])
clean_housing['date_mutation'] = clean_housing['date_mutation'].apply(lambda x:x.toordinal())
clean_housing.head()


# In[16]:


np.random.seed(42)


# In[17]:




from sklearn.model_selection import train_test_split
train_set, test_set = train_test_split(clean_housing, test_size=0.2, random_state=42, stratify=clean_housing[['code_postal']])

#from sklearn.model_selection import StratifiedShuffleSplit

#split = StratifiedShuffleSplit(n_splits=1, test_size=0.2, random_state=42)
#for train_index, test_index in split.split(clean_housing, clean_housing["Code postal"]):
#    train_set = clean_housing.reindex(train_index)
#    test_set = clean_housing.reindex(test_index)


# In[18]:


corr_matrix=clean_housing.corr()


# In[19]:


corr_matrix["valeur_fonciere"].sort_values(ascending=False)


# In[20]:


list(clean_housing)


# In[21]:


#train_set.dropna(subset=["Valeur fonciere"], inplace=True)
#test_set.dropna(subset=["Valeur fonciere"], inplace=True)
print "Train set info"
train_set.info()

print "Test set info"
test_set.info()


# In[22]:


train_set.plot(kind="scatter", x="longitude", y="latitude")


# In[23]:


from sklearn.pipeline import Pipeline
from sklearn.preprocessing import StandardScaler
from sklearn.impute import SimpleImputer
from sklearn.compose import ColumnTransformer
from sklearn.preprocessing import OneHotEncoder
from sklearn_pandas import CategoricalImputer
from sklearn.model_selection import GridSearchCV
from sklearn.ensemble import RandomForestRegressor
from sklearn.base import BaseEstimator, TransformerMixin
from sklearn.model_selection import RandomizedSearchCV
from scipy.stats import randint

# Create a class to select numerical or categorical columns 
class DataFrameSelector(BaseEstimator, TransformerMixin):
    def __init__(self, attribute_names):
        self.attribute_names = attribute_names
    def fit(self, X, y=None):
        return self
    def transform(self, X):
        return X[self.attribute_names].values

    
train_housing = train_set.drop("valeur_fonciere", axis=1) # drop labels for training set
train_housing_labels = train_set["valeur_fonciere"].copy()

#num_attribs=["Date mutation","No voie","Nombre de lots", "Surface Carrez du 4eme lot","Surface Carrez du 1er lot", "Surface Carrez du 3eme lot", "Surface Carrez du 2eme lot", "Surface Carrez du 5eme lot", "Code type local", "Code postal", "Surface reelle bati", "Surface terrain", "Nombre pieces principales"]
#cat_attribs=["Code voie","Type de voie"]

num_attribs=["date_mutation","code_postal","lot1_surface_carrez","lot2_surface_carrez","lot3_surface_carrez","lot4_surface_carrez","lot5_surface_carrez","nombre_lots","code_type_local","surface_reelle_bati","nombre_pieces_principales","surface_terrain","longitude","latitude"]

#num_attribs=["Date mutation","Nombre de lots", "Surface Carrez du 4eme lot","Surface Carrez du 1er lot", "Surface Carrez du 3eme lot", "Surface Carrez du 2eme lot", "Surface Carrez du 5eme lot", "Code type local", "Code postal", "Surface reelle bati", "Surface terrain", "Nombre pieces principales"]
#cat_attribs=["Code voie","Type de voie"]
cat_attribs=["Section"]

#num_attribs=["Surface Carrez du 1er lot", "Prefixe de section", "Code postal", "Surface reelle bati", "Surface terrain",  "Nombre pieces principales"]
#cat_attribs=["Voie", "Commune", "Type local"]


num_pipeline = Pipeline([
        ('selector', DataFrameSelector(num_attribs)),
        ('imputer', SimpleImputer(strategy="median")),
        ('std_scaler', StandardScaler()),
    ])
cat_pipeline = Pipeline([
        ('selector', DataFrameSelector(cat_attribs)),
        ('cat_imputer', CategoricalImputer()),
        ('cat_to_id', OneHotEncoder()),
    ])
#full_pipeline = ColumnTransformer([
#        ("num", num_pipeline, num_attribs),
#        ("cat", cat_pipeline, cat_attribs),
#    ])

full_pipeline = ColumnTransformer([
        ("num", num_pipeline, num_attribs)
    ])


train_housing_prepared = full_pipeline.fit_transform(train_housing)

#param_grid = [
#    # try 12 (3×4) combinations of hyperparameters
#    {'n_estimators': [3, 10, 30], 'max_features': [2, 4, 6, 8]},
#    # then try 6 (2×3) combinations with bootstrap set as False
#    {'bootstrap': [False], 'n_estimators': [3, 10], 'max_features': [2, 3, 4]},
#  ]

#forest_reg = RandomForestRegressor(random_state=42)
# train across 5 folds, that's a total of (12+6)*5=90 rounds of training 
#grid_search = GridSearchCV(forest_reg, param_grid, cv=5,
#                           scoring='neg_mean_squared_error', return_train_score=True)


param_distribs = {
        'n_estimators': randint(low=1, high=200),
        'max_features': randint(low=1, high=8),
    }
#param_distribs = {
#        'n_estimators': randint(low=150, high=250),
#        'max_features': randint(low=6, high=12),
#    }

#param_distribs = {
#        'n_estimators': [180],
#        'max_features': [7],
#    }

forest_reg = RandomForestRegressor(random_state=42, oob_score=True, n_jobs=4)
rnd_search = RandomizedSearchCV(forest_reg, param_distributions=param_distribs,
                                n_iter=10, cv=5, scoring='neg_mean_squared_error', random_state=42)
#rnd_search = RandomizedSearchCV(forest_reg, param_distributions=param_distribs,
#                                n_iter=1, cv=5, scoring='neg_mean_squared_error', random_state=42)


# In[24]:


train_housing.head()
#train_housing_labels.head()
#list(train_housing)
#np.isnan(train_housing_labels)
#np.where(np.isnan(train_housing_labels))
#train_housing_labels['169']
#train_set.isna()


# In[25]:


import time
start_time = time.time()
rnd_search.fit(train_housing_prepared, train_housing_labels)
elapsed_time = time.time() - start_time
time.strftime("%H:%M:%S", time.gmtime(elapsed_time))

#grid_search.fit(train_housing_prepared, train_housing_labels)


# In[29]:


final_model = rnd_search.best_estimator_
#final_model = grid_search.best_estimator_


# In[30]:


some_data = train_housing.iloc[:10]
some_labels = train_housing_labels.iloc[:10]
some_data_prepared = full_pipeline.transform(some_data)

print("Predictions:", final_model.predict(some_data_prepared))
print("Labels:", list(some_labels))

#from sklearn.externals import joblib
#joblib.dump(final_model, "/home/bonpiedlaroute/boulot/data/model_paris_18_17.pkl") # DIFF
#...
#my_model_loaded = joblib.load("my_model.pkl") # DIFF


# In[31]:


train_housing.head(10)


# In[32]:



cvres = rnd_search.cv_results_
for mean_score, params in zip(cvres["mean_test_score"], cvres["params"]):
    print(np.sqrt(-mean_score), params)

#cvres = grid_search.cv_results_
#for mean_score, params in zip(cvres["mean_test_score"], cvres["params"]):
#    print(np.sqrt(-mean_score), params)

final_model


# In[33]:


from sklearn.metrics import mean_squared_error
#housing_predictions = grid_search.predict(train_housing_prepared)
housing_predictions = final_model.predict(train_housing_prepared)
mse = mean_squared_error(train_housing_labels, housing_predictions)
rmse = np.sqrt(mse)
rmse


# In[34]:


final_model.score(train_housing_prepared, train_housing_labels)


# In[36]:



test_housing = test_set.drop("valeur_fonciere", axis=1) # drop labels for test set
test_housing_labels = test_set["valeur_fonciere"].copy()

test_data = test_housing.iloc[:10]
test_labels = test_housing_labels.iloc[:10]
test_data_prepared = full_pipeline.transform(test_data)

print("Predictions:", final_model.predict(test_data_prepared))
print("Labels:", list(test_labels))

#test_housing_prepared = full_pipeline.fit_transform(test_housing)
#test_predictions = rnd_search.predict(test_housing_prepared)
#test_mse = mean_squared_error(test_housing_labels, test_predictions)
#test_rmse = np.sqrt(test_mse)
#test_rmse


# In[37]:


test_housing.info()


# In[38]:


test_housing.head(10)


# In[39]:


test_housing_prepared = full_pipeline.transform(test_housing)
test_predictions = final_model.predict(test_housing_prepared)
test_mse = mean_squared_error(test_housing_labels, test_predictions)
test_rmse = np.sqrt(test_mse)
test_rmse


# In[40]:


final_model.score(test_housing_prepared,test_housing_labels)


# In[41]:


final_model.oob_score_


# In[42]:


#from sklearn.externals import joblib
#joblib.dump(final_model, "/home/bonpiedlaroute/boulot/data/model_paris_all_geo_05062019.pkl")


# In[28]:


from sklearn.externals import joblib
my_model_loaded = joblib.load("/home/bonpiedlaroute/boulot/data/model_paris_all_geo_05062019.pkl")


# In[31]:


from sklearn.metrics import mean_squared_error

test_housing = test_set.drop("valeur_fonciere", axis=1) # drop labels for test set
test_housing_labels = test_set["valeur_fonciere"].copy()
test_housing_prepared = full_pipeline.transform(test_housing)
test_predictions = my_model_loaded.predict(test_housing_prepared)
test_mse = mean_squared_error(test_housing_labels, test_predictions)
test_rmse = np.sqrt(test_mse)
test_rmse


# In[32]:


my_model_loaded.score(test_housing_prepared,test_housing_labels)


# In[ ]:




