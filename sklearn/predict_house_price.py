import os
import pandas as pd
HOUSING_PATH = "/home/bonpiedlaroute/boulot/data"
def load_housing_data(housing_path = HOUSING_PATH):
    #csv_data = os.path.join(housing_path, "valeursfoncieres-2018.txt")
    csv_data = os.path.join(housing_path, "paris_2018.txt")
    return pd.read_csv(csv_data, sep='|', dtype={"Valeur fonciere": float}, decimal=',')
housing = load_housing_data()
housing.head()

housing.info()

clean_housing=housing.drop(["Code service CH", "Reference document","1 Articles CGI","2 Articles CGI","3 Articles CGI","4 Articles CGI", "5 Articles CGI"], axis=1)

clean_housing = clean_housing.drop(["B/T/Q"], axis=1)

clean_housing = clean_housing.drop(["Identifiant local"], axis=1)

clean_housing.info()

clean_housing.describe()

clean_housing = clean_housing.dropna(subset=["Code postal"])

clean_housing=clean_housing.dropna(subset=["Valeur fonciere"])

clean_housing.info()

clean_housing.head()

from sklearn.model_selection import train_test_split
train_set, test_set = train_test_split(clean_housing, test_size=0.2, random_state=42)

corr_matrix=clean_housing.corr()

corr_matrix["Valeur fonciere"].sort_values(ascending=False)

clean_housing.info()

corr_matrix=clean_housing.corr()
corr_matrix["Valeur fonciere"].sort_values(ascending=False)

clean_housing.head(10)

clean_housing.info()

list(clean_housing)

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

    
train_housing = train_set.drop("Valeur fonciere", axis=1) # drop labels for training set
train_housing_labels = train_set["Valeur fonciere"].copy()

num_attribs=["Surface Carrez du 4eme lot","Surface Carrez du 1er lot", "Surface Carrez du 3eme lot", "Surface Carrez du 2eme lot", "Surface Carrez du 5eme lot", "Prefixe de section", "Code type local", "Code postal", "Surface reelle bati", "4eme lot", "Surface terrain", "No plan", "Nombre pieces principales"]
cat_attribs=["Voie", "Commune", "Type local"]
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
full_pipeline = ColumnTransformer([
        ("num", num_pipeline, num_attribs),
        ("cat", cat_pipeline, cat_attribs),
    ])
train_housing_prepared = full_pipeline.fit_transform(train_housing)

param_grid = [
    # try 12 (3×4) combinations of hyperparameters
    {'n_estimators': [3, 10, 30], 'max_features': [2, 4, 6, 8]},
    # then try 6 (2×3) combinations with bootstrap set as False
    {'bootstrap': [False], 'n_estimators': [3, 10], 'max_features': [2, 3, 4]},
  ]

forest_reg = RandomForestRegressor(random_state=42)
# train across 5 folds, that's a total of (12+6)*5=90 rounds of training 
#grid_search = GridSearchCV(forest_reg, param_grid, cv=5,
#                           scoring='neg_mean_squared_error', return_train_score=True)
#grid_search.fit(train_housing_prepared, train_housing_labels)


#final_model = grid_search.best_estimator_



param_distribs = {
        'n_estimators': randint(low=1, high=200),
        'max_features': randint(low=1, high=8),
    }

forest_reg = RandomForestRegressor(random_state=42, n_jobs=3)
#rnd_search = RandomizedSearchCV(forest_reg, param_distributions=param_distribs,
#                                n_iter=10, cv=5, scoring='neg_mean_squared_error', random_state=42)
rnd_search = RandomizedSearchCV(forest_reg, param_distributions=param_distribs,
                                n_iter=10, cv=5, scoring='neg_mean_squared_error', random_state=42)


rnd_search.fit(train_housing_prepared, train_housing_labels)

final_model = rnd_search.best_estimator_

some_data = train_housing.iloc[:10]
some_labels = train_housing_labels.iloc[:10]
some_data_prepared = full_pipeline.transform(some_data)

print("Predictions:", final_model.predict(some_data_prepared))
print("Labels:", list(some_labels))

#from sklearn.externals import joblib
#joblib.dump(my_model, "my_model.pkl") # DIFF
#...
#my_model_loaded = joblib.load("my_model.pkl") # DIFF

import numpy as np
icvres = rnd_search.cv_results_
for mean_score, params in zip(cvres["mean_test_score"], cvres["params"]):
    print(np.sqrt(-mean_score), params)
final_model
