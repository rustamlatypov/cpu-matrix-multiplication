n = c(2000, 3000, 4000, 5000, 6000, 7000, 8000)
base = c(31.24, 135.16, 372.68, 812.09, 1454.66, 2604.8, 5037) + 1
para = c(0.121, 0.403, 0.946, 2.17, 3.87, 6.72, 12.38) + 1

data <- data.frame(n, base, para)

#log = "y" 

par(mar = c(5,5,5,5))
plot(data$n, data$base, type="b", xlab = "n", ylab = "seconds", bty = "n", log = "y",
     lwd = 1.6, cex = 1.2, cex.axis = 1.3, cex.lab = 1.3, ylim=c(0.9,6000))
lines(data$n, data$para, type="b", lwd = 1.6, cex = 1.2)