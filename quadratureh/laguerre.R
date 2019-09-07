# Learn the dimension -----------------------------------------------------
# class: STAT580
# authors: SSGL
# version: 0.1



# This code is ot get absciccas and weights for Laguerre polynomial.
# For the best result we assume n_max = 1000
library(gaussquad)
library(purrr)
n <- 1000

# store absciccas and weights into table
table <- laguerre.quadrature.rules(n, FALSE)
map_depth(.x = table, 1, data.frame) %>%  
    map_dfr(data.frame, .id = "n") -> laguerre
laguerre[,-1] %>% dim()

# write laguerre table into a txt file, omit first column
write.table(laguerre[,-1], 
          file = "laguerre.txt", 
          row.names = FALSE, 
          #omitting col_names, to easily read data in c 
          col.names = FALSE)


# check functions
f1 <- function(t, alpha, x_min) {
    res = t^(alpha-1) * exp((1-x_min)*t) / (1-exp(-t));
    return(res);
}

laguerre.quadrature(functn = f1,
                    rule = table[[1000]],
                    lower = 0,
                    upper = Inf,
                    alpha = 0.5,
                    x_min = 1)

















