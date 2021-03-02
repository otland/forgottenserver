Game.createItemType(26378):register({
   name = "gold converter",
   article = "a",
   weight = 1800,
   duration = 604800,
   charges = 500,
   showcharges = true,
   showduration = true
})

-- Exaple for fields
--[[Game.createItemType(1487):register({
   name = "fire field",
   article = "a",
   type = "magicfield",
   decayTo = 1488,
   duration = 120,
   field = {
      name = "fire",
      damage = 20,
      ticks = 10000,
      count = 7
   }
})]]

-- Example for multiIds
--[[Game.createItemType(20005, 20080):register({
   name = "sign",
   article = "a",
   allowDistRead = "true"
})]]
