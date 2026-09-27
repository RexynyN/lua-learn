# cotton-ml
The fluffliest Machine Learn library you'll ever find


# Goal

Implement the maximum amount of ML Models

### 1. Supervised Learning: Classification

Used to predict a discrete label or category (e.g., Spam vs. Not Spam).

| Category | Key Models |
| --- | --- |
| **Linear Models** | `LogisticRegression`, `RidgeClassifier`, `SGDClassifier`, `Perceptron` |
| **Support Vector Machines** | `SVC`, `LinearSVC`, `NuSVC` |
| **Nearest Neighbors** | `KNeighborsClassifier`, `RadiusNeighborsClassifier` |
| **Naive Bayes** | `GaussianNB`, `MultinomialNB`, `BernoulliNB`, `ComplementNB` |
| **Trees & Ensembles** | `DecisionTreeClassifier`, `RandomForestClassifier`, `GradientBoostingClassifier`, `HistGradientBoostingClassifier`, `AdaBoostClassifier` |
| **Neural Networks** | `MLPClassifier` (Multi-layer Perceptron) |

---

### 2. Supervised Learning: Regression

Used to predict a continuous numerical value (e.g., Housing prices).

| Category | Key Models |
| --- | --- |
| **Linear Models** | `LinearRegression`, `Ridge`, `Lasso`, `ElasticNet`, `BayesianRidge`, `HuberRegressor` |
| **Support Vector Machines** | `SVR`, `LinearSVR` |
| **Nearest Neighbors** | `KNeighborsRegressor` |
| **Trees & Ensembles** | `DecisionTreeRegressor`, `RandomForestRegressor`, `GradientBoostingRegressor`, `VotingRegressor`, `StackingRegressor` |
| **Neural Networks** | `MLPRegressor` |

---

### 3. Unsupervised Learning: Clustering

Used to group unlabeled data based on patterns.

* **Centroid-based:** `KMeans`, `MiniBatchKMeans`
* **Density-based:** `DBSCAN`, `OPTICS`
* **Hierarchical:** `AgglomerativeClustering`, `Birch`
* **Distribution-based:** `GaussianMixture`
* **Graph-based:** `SpectralClustering`, `AffinityPropagation`

---

### 4. Dimensionality Reduction

Used to compress data or find underlying structures.

* **Matrix Factorization:** `PCA` (Principal Component Analysis), `IncrementalPCA`, `KernelPCA`
* **Manifold Learning:** `TSNE`, `Isomap`
* **Decomposition:** `NMF` (Non-negative Matrix Factorization), `DictionaryLearning`

---

### 5. Specialized Estimators

* **Anomaly Detection:** `IsolationForest`, `OneClassSVM`, `LocalOutlierFactor`.
* **Semi-Supervised:** `LabelPropagation`, `SelfTrainingClassifier`.
* **Multi-output:** `MultiOutputClassifier`, `RegressorChain`.



# TODOs

- CSV Parsing
    - [x] Whole text Parsing
    - [x] Parsing to Matrix 
    - [x] Parsing to Dataset

- KNN 
    - [x] Naive Approach (Brute force - Distance Based)
    - [x] Multiple distances implemented
    - [ ] KD-Tree Implementation
    - [ ] Ball Tree Implementation 

- Naive Bayes 
    - [ ] Simple Implementation
