# Learn Sequence of FASTQ File -----------------------------------------------------
# class: STAT580
# authors: SSGL
# version: 0.1

# Read Data ------------------------------------------------------------------------

source("http://www.bioconductor.org/biocLite.R")
biocLite(c("ShortRead"))
require("ShortRead")
require("Biostrings")

setwd("/home/sepidehmosaferi/Desktop") #Give the directory which stores data
DATA <- readFastq("SRR2990088_1_noN.fastq.txt")
length(DATA)

SEQ <- sread(DATA)   #For Reading the Nucleotide Sequences
SCORE <- quality(DATA)  #For Reading the Quality Scores
id(DATA)  #For Reading the IDs

kmercounts <- oligonucleotideFrequency(SEQ,4)  #Take k=4
head(kmercounts)

# Whole Sequences -------------------------------------------------------------------

FREQ <- apply(kmercounts,2,sum)
length(FREQ)

# Write it as a .txt File -----------------------------------------------------------

write.table(FREQ, 
            file = "sequence.txt", 
            sep = " ",
            col.names = FALSE,
            row.names = FALSE)

# [part b] Bisection Method ----------------------------------------------------------

alphahatfunc <- function(alpha,xvec){
  n <- length(xvec);
  eq <- -n*digamma(alpha)+sum(log(xvec));
  eq;
}

alphahatfunc(2,FREQ)  # Take alpha=2 as Initial Value

alpharoot <- uniroot(alphahatfunc,interval=c(min(FREQ),max(FREQ)),xvec=FREQ)
alpharoot

# From the Above Code, We Can Find the MLE Equals to "942230.2"!
# ------------------------------------------------------------------------------------
# Additional Information for the Bisection Method in C Program -----------------------

a <- min(FREQ); a  #a is "28329"!
b <- max(FREQ); b  #b is "7344124"!

# [part c]  Maximizing the BIC when xmin is not equal to 1 ---------------------------

# First, Define a possible range for the xmin.
# Let's assume the range of 1:length(FREQ) as the possible values for xmin.
XMIN <- 1:length(FREQ)

# Bisection Method -------------------------------------------------------------------
alphahatfunc2 <- function(alpha,xvec,xmin){
  n <- length(xvec);
  eq <- -n*digamma(alpha)+sum(log(xvec/xmin));
  eq;
}

# Generalize the uniroot function ----------------------------------------------------

find_root <- function(f, l){
  uniroot(
    function(alpha) alphahatfunc2(alpha, f, l),
    interval = c(min(FREQ),max(FREQ)), extendInt = "yes"
  )$root
}

# Loop the uniroot on the dataset of values of FREQ and XMIN -------------------------
FREQ <- as.vector(FREQ)
DATA <- as.data.frame(cbind(rep(FREQ,length(FREQ)),rep(XMIN,each=length(XMIN))))

colnames(DATA) <- c("FREQ", "XMIN")

# Write it as a .txt File -----------------------------------------------------------

write.table(cbind(DATA$FREQ,DATA$XMIN), 
            file = "sequencepartc.txt", 
            sep = " ",
            col.names = FALSE,
            row.names = FALSE)

# Final Answer [The results] ---------------------------------------------------------

DATA$root <- mapply(find_root,DATA$FREQ,DATA$XMIN)
head(DATA)
DATA[DATA$root==max(DATA$root),]

# From the Above Code, We Can Find the MLE Equals to "7344125" 
# and appropriate xmin is "1"!
# FREQ       XMIN   root
# 7344124    1      7344124

# Assumptions for finding the BIC -----------------------------------------------------
# 1) For x<xmin, define Px_i= x_i/sum(x_i), which is zero here.
# 2) Take "a" as a constant; Let's a:= 2
# 3) P(x_i) = x_i^a * {1-e^(-x_i)}/x_min^-alpha
# 4) BIC = 2 * sum[log(P(x_i))]-xmin log(n)

XMIN <- 1
alpharoot <- 7344125 
a <- 2 
  
Px_i <- FREQ^a *{1-exp(-FREQ)}/XMIN^{-alpharoot}
BIC <- 2*sum(log(Px_i))-XMIN*log(length(FREQ)); BIC

# Maximum BIC is "14080.6"!!
# ======================================================================================
