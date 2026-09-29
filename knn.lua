local KNN = {}
KNN.__index = KNN

local KNNDistance = {
    EuclidDistance = 0,
    ManhattanDistance = 1,
    MinkowskiDistance = 2,
    CosineDistance = 3,
    HammingDistance = 4
};


function KNN:new()
    local instance = {
        kNeighbors = 5,
        distance = KNNDistance.MinkowskiDistance,

    }
    setmetatable(instance, self)
    return instance
end


function KNN:set(prop, value)
    self[prop] = value
    return self
end
