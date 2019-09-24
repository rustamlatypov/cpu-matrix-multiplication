n = c(2000, 3000, 4000, 5000, 6000, 7000, 8000)
base = c(31.71, 134.78, 366.72, 775.73, 1428.5, 2462.2, 5086.6) + 1
para = c(0.104, 0.35, 0.839, 1.651, 2.836, 4.531, 7.495) + 1

data <- data.frame(n, base, para)

#log = "y" 

par(mar = c(5,5,5,5))
plot(data$n, data$base, type="b", xlab = "n", ylab = "seconds", bty = "n", log = "y",
     lwd = 1.6, cex = 1.2, cex.axis = 1.3, cex.lab = 1.3, ylim=c(0.9,6000))
lines(data$n, data$para, type="b", lwd = 1.6, cex = 1.2)


text(5000, 1800, "Sequential", cex = 1.3)
text(4830, 5.4, "Parallel", cex = 1.3)
