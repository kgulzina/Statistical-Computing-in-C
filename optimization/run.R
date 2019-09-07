# Setup here
filepath <- "~/Dropbox/coursework/STAT580/home/hw6"
filename <- "main"

# Let it run --------------------------------------------------------------

# Compile shared library if not found
cfile    <- file.path(filepath, sprintf("%s.c", filename))
libfile  <- file.path(filepath, sprintf("%s.%s", filename, "so"))

if (!file.exists(libfile)) {
  cat("Couldn't find the shared library, will build if for you.\n")
  cat("Yes, this program is that invasive that it makes decisions without asking the user Y/n.\n")

  if (!file.exists(cfile))
    stop("Couldn't find lib nor source files. Can't do much sorry o/")

  system(
    sprintf(
      "R CMD SHLIB %s -o %s",
      cfile, libfile
    )
  )
}

# Define wrapper
newton <- function(initial, epsilon, maxiter) {
  .Call("rnewton", as.double(initial), as.double(epsilon), as.integer(maxiter))
}

# Load, run, enjoy!
dyn.load(libfile)
s <- newton(0, 1E-6, 1000)
cat(
  sprintf("Result: % .6e\n", s)
)
dyn.unload(libfile)
