n = c(2000, 3000, 4000, 5000, 6000, 7000)
base = c(31.71, 134.78, 366.72, 775.73, 1428.5, 2462.2)
para = c(0.098, 0.321, 0.775, 1.511, 2.605, 4.477)

data <- data.frame(n, base, para)

par(mar = c(5,5,5,5))
plot(data$n, data$base, type="b", xlab = "n", ylab = "seconds", bty = "n", log = "y",
     lwd = 1.6, cex = 1.2, cex.axis = 1.3, cex.lab = 1.3, ylim=c(0.1,6000))
lines(data$n, data$para, type="b", lwd = 1.6, cex = 1.2)


text(5000, 1800, "Sequential", cex = 1.3)
text(4830, 5.4, "Parallel", cex = 1.3)
