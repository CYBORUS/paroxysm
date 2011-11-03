function randomLocation()
    return math.random() * 9
end

function allTheThings()
    for i = 1, 10 do
        local e = addEntity()
        addActor(e, "bradley.c3m")
        setEntityPosition(e, randomLocation(), randomLocation(), 1)
    end
end

allTheThings()
