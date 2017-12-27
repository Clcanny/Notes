module Main 
(
  main
) where

import SimpleJSON(JValue(..))
import PrettyJSON(renderJValue)
import Doc(pretty)

put x y = putStrLn (pretty x (renderJValue y))

main = put 10 (JObject [("foo", JNumber 1), ("bar", JBool False)])
