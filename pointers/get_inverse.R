# Learn the dimension -----------------------------------------------------
# class: STAT580
# authors: SSGL
# version: 0.1



# Read the data -----------------------------------------------------------
library(MASS)

data <- read.table("cov.txt")

# since it is list, convert it to data frame
v <- as.numeric(unlist(data))
cov <- matrix(v, ncol = 10, byrow = TRUE)
inv_cov <- solve(cov)

# write it as a .txt file
write.table(inv_cov, 
            file = "inverse.txt", 
            sep = " ",
            col.names = FALSE,
            row.names = FALSE)
# end ---------------------------------------------------------------------


