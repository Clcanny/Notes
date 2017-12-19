data Customer = Customer {
    customerID :: Int
  , customerName :: String
  , customerAddress :: String
} deriving (Show)

customer = Customer 1 "Demons" "NJU"
id = customerID customer
name = customerName customer
address = customerAddress customer

data Color = 
    Red { fx :: Int }
  | Yellow { fy :: Int }
  | Blue { fz :: Int }
    deriving (Show)

red = Red 1
yellow = Yellow 2
blue = Blue 3

x = fx red
y = fy yellow
z = fz blue

anotherRed = Red { fx = 1 }
anotherCustomer = Customer {
    customerID = 2
  , customerAddress = "NJU"
  , customerName = "Vivian"
}
