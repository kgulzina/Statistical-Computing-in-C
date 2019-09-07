# Newton and Fisher Scoring Methods ------------------------------------------------
# class: STAT580
# authors: SSGL
# version: 0.1


##     _    __       _____
##    / \   | |     / ____|         Iterative
##   / _ \  | |     | |  _          Algorithms
##  / ___ \ | |____ | |_| |         [Newton's & Fisher]
## /_/   \_\|_|____|\_____/         version 0.1



# Read Data ------------------------------------------------------------------------
DATA <- read.table("oil.Rtxt", header = TRUE)

# Initial Values:
Beta <- glm(DATA$spills ~ DATA$importexport+DATA$domestic-1, poisson)$coefficients  
Beta_Newton <- Beta; 

# ALGORITHMS ========================================================================

# Newton's Algorithm ----------------------------------------------------------------
rep <- 1
repeat{
  
  Gradian1_N <- -sum(DATA$importexport)+sum(DATA$spills*DATA$importexport/
                                                 (Beta_Newton[1]*DATA$importexport+Beta_Newton[2]*DATA$domestic))
  
  Gradian2_N <- -sum(DATA$domestic)+sum(DATA$spills*DATA$domestic/
                                                 (Beta_Newton[1]*DATA$importexport+Beta_Newton[2]*DATA$domestic))
  
  Gradian_N <- matrix(c(Gradian1_N,Gradian2_N), nrow = 2)
  
  Newton11 <- sum(DATA$importexport^2 * DATA$spills/(Beta_Newton[1]*DATA$importexport+
                                                       Beta_Newton[2]*DATA$domestic)^2)
  
  Newton12.21 <- sum(DATA$spills * DATA$importexport * DATA$domestic/(Beta_Newton[1]*DATA$importexport+
                                                       Beta_Newton[2]*DATA$domestic)^2)
  
  Newton22 <- sum(DATA$domestic^2 * DATA$spills/(Beta_Newton[1]*DATA$importexport+
                                                       Beta_Newton[2]*DATA$domestic)^2) 
  
  Newton <- matrix(c(Newton11,Newton12.21,Newton12.21,Newton22), nrow = 2, ncol = 2, byrow = TRUE)
  
  Betanew_Newton <- Beta_Newton + solve(Newton) %*% Gradian_N
  
  Norm_Newton <- sqrt((Betanew_Newton[1]-Beta_Newton[1])^2+(Betanew_Newton[2]-Beta_Newton[2])^2)
  
  Rel_Conv.N <- Norm_Newton/ sqrt(Beta_Newton[1]^2+Beta_Newton[2]^2) 
  
  Beta_Newton <- Betanew_Newton
  
  epsilon <- 10^(-6) 
 
  if(Rel_Conv.N < epsilon) break
  
  cat("On iteration", rep, ", Beta's [1,2] =", round(Beta_Newton,6), '\n')
  rep <- rep+1
  
}
# ===================================================================================

# Scoring Algorithm -----------------------------------------------------------------
rep <- 1
Beta_Score <- Beta

while(rep <= 100){
  
  Gradian1_S <- -sum(DATA$importexport)+sum(DATA$spills*DATA$importexport/
                                              (Beta_Score[1]*DATA$importexport+Beta_Score[2]*DATA$domestic))
  
  Gradian2_S <- -sum(DATA$domestic)+sum(DATA$spills*DATA$domestic/
                                          (Beta_Score[1]*DATA$importexport+Beta_Score[2]*DATA$domestic))
  
  Gradian_S <- matrix(c(Gradian1_S,Gradian2_S), nrow = 2)
  
  Score11 <- sum(DATA$importexport^2/(Beta_Score[1]*DATA$importexport+
                                        Beta_Score[2]*DATA$domestic)) 
  
  Score12.21 <- sum(DATA$importexport * DATA$domestic/(Beta_Score[1]*DATA$importexport+
                                                         Beta_Score[2]*DATA$domestic)) 
  
  Score22 <- sum(DATA$domestic^2/(Beta_Score[1]*DATA$importexport+
                                    Beta_Score[2]*DATA$domestic)) 
  
  Score <- matrix(c(Score11,Score12.21,Score12.21,Score22), nrow = 2, ncol = 2, byrow = TRUE)
  
  Betanew_Score <- Beta_Score + solve(Score) %*% Gradian_S
  
  Norm_Score <- sqrt((Betanew_Score[1]-Beta_Score[1])^2+(Betanew_Score[2]-Beta_Score[2])^2) 
  
  Rel_Conv.S <- Norm_Score/ sqrt(Beta_Score[1]^2+Beta_Score[2]^2)  
  
  Beta_Score <- Betanew_Score
  
  epsilon <- 10^(-6) 

  if(Rel_Conv.S < epsilon) break
  
  cat("On iteration", rep, ", Beta's [1,2] =", round(Beta_Score,6), '\n')
  rep <- rep+1

}
# ===================================================================================





