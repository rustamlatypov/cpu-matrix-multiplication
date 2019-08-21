n = c(1000, 2000, 3000, 4000, 5000, 6000)
base = c(1.43, 31.24, 135.16, 372.68, 812.09, 1454.66) + 1
para = c(0.0172, 0.121, 0.403, 0.946, 2.17, 3.87) + 1

data <- data.frame(n, base, para)

#log = "y" 

par(mar = c(5,5,5,5))
plot(data$n, data$base, type="b", xlab = "n", ylab = "seconds", bty = "n", log = "y",
     lwd = 1.6, cex = 1.2, cex.axis = 1.3, cex.lab = 1.3, ylim=c(0.9,700))
lines(data$n, data$para, type="b", lwd = 1.6, cex = 1.2)