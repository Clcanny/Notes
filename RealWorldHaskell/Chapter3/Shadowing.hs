bar = let x = 1
      in ((let x = "foo" in x), x)

quux a = let a= "foo"
         in a ++ "eek!"

str = quux 1
