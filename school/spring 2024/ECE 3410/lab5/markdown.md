---
title: 'Lab 5 report'
Author: 'Nathan Walker'
...

R~D~:

$$ A~CS~ = (-g_m * r~o~) * \frac{R~D~ || R~L}{R~out + R~D~ || R~L~} where R~out = R~S~ + r~o~ + g~m~ * r~o~ * R~S~ $$

since r~o~ goes to $\infty$, the R~out~ term grows higher than the parallel term.

$$ A~CS~ = (-g~m~ * r~o~) * \frac{R~D~ || R~L}{g~m~ * r~o~ * R~S~} $$

Then the terms will cancel out from the denominator.

$$ A~CS~ = -\frac{R~D~ || R~L}{R~S~} $$

solving: $R_D || R_L = 1k$ $\Omega$ therefore $R_D = 2k$$\Omega$

R~S~:

$$ A_SF = \frac{R~D~ || R~L}{R~out + R~D~ || R~L~} = \frac{R~D~ || R~L}{\frac{1}{g_m} + R~D~ || R~L~} $$

Solving: R_S || R_L = $\frac{1}{g_m}$ = 1k$\Omega$ therfore R_S = 2k$\Omega$ 

I~D~: Next, I~D~ can be solved using the equation 

$$g_m = \sqrt{\mu C_ox (W/L) I_D} \rightarrow I_D = 1.502 mA $$

V~S1~: Given: $g_m = K_n * V_{ov}$ $\rightarrow$ $V_{ov} = \frac{g_m}{K_n} = 3.003V$

V~GS~: $V_{GS} = V_{ov} + V_{th} = 5.003V$

V~G~: $V_G = V_S + V_{GS} = 8.006V$

V~D1~: $V_{S1} = 3.003V \rightarrow V_{D1} = V_{DD} - V_{S1} = 6.997V$

$V_{DS1}: V_{DS1} = V_{D1} - V_{S1} = 3.994V$ 

M1 Operating Region: The operating region is saturation with $V_{DS}$ > $V_{ov} \rightarrow 3.994V$ > $3.003V$

## Exercise 14

$$ g_{m2} = 1.5 mA/V$$
$$ A_{CG} = 1V/V, where A_{CG} = \frac{v_{g1}}{v_{s2}}$$
$$ R_{in} = 300\Omega $$

Derive an expresion for R~in~ and solve for R~2~

$$ R_{in} = R_2 || \frac{1}{g_{m2}} \rightarrow \frac{\frac{R_2}{g_{m2}}}{R_2 + \frac{1}{g_{m2}}} = 300\Omega \rightarrow R_2 = \frac{\frac{300}{g_{m2}}}{-300 + \frac{1}{g_{m2}}}$$

$$A_{CG} = g_{m2}R_3 \rightarrow R_3 = 667\Omega$$

$$I_D = \frac{g_{m2}^2}{2K_n} = 4.505V$$

$$V_{ov2} = \frac{g_{m2}}{K_n} = 4.505V$$

$$V_{GS2} = V_{ov2} + V_{th} = 6.505V$$

$$V_{S2} = I_{D2}R_2 = 1.84V $$

$$ V_{G2} = V_{S2} + V_{GS2} = 8.34V $$

$$ V_{D2} = V_{DD} - I_{D2}R_3 = 7.75V $$

$$ V_{DS2} = V_{D2} - V_{S2} = 5.9V $$

The M2 operating region is saturation. $V_{DS2}$ > $V_{ov2} \rightarrow 5.9V$ > $4.5V$

# Simulation

## Exercise 15

$$ \Delta A = 20log10(\frac{A_{CS+A_{SF}}}{0.001}) $$

$$ \Delta \phi = 2\pi f \Delta t $$

Using the formulas above and the simulation in the appendix these values were calculated. 

| Parameter | Value |
| :----:    | :----:|
| v~o1~     |255.93mV|
| v~o2~     |255.93mV|
| v~in~     |999.99mV|
| t~o~(v~o1~)|9.9595$\mu$s|
| t~o~(v~o1~)|9.9598$\mu$s|
| $\Delta$A|54.18dBm|
| $\Delta \phi$     |-83.252$\mu$rad|

Viewing both the phase imbalance and the amplitude imbalance we can see that the circuit is indeed matched and outputs a differential output. 

INSERT PLOTS AND TALK ABOUT IT

\newpage

# Physical Experiments

## Procedure 1 and 2

| Parameter | Design Prediction | Adjusted Predictions | Actual Measurements |
| :----:    | :----:            |       :----:         |    :----:           |
| R~1~|100k$\Omega$ | 97.750k$\Omega$ | 97.750k$\Omega$ | 
| R~D~|2k$\Omega$ | 1.9902k$\Omega$ | 1.9902k$\Omega$ | 
| R~S~|2k$\Omega$ | 2.0001k$\Omega$ | 2.0001k$\Omega$ | 
| R~L1~|2k$\Omega$ | 1.9791k$\Omega$ | 1.9791k$\Omega$ | 
| R~L2~|2k$\Omega$ | 1.9862k$\Omega$ | 1.9862k$\Omega$ | 
| V~G1~ | 8.006V | 7.6695V | 7.8286V | 
| V~S1~ | 3.003V | 2.7801V | 2.8099V | 
| V~D1~ | V | 7.2199V | 7.203V | 
| I~D1~ | 1.502mA | 1.502mA | 1.39mA|

# Procedure 3

INSERT PLOT AND TALK ABOUT IT.

# Procedure 4

Using measurements from the resistors from the experiment the gain of the amplifier is calculated.

$$ A_{CS} = 0.4961V/V $$

$$ A_{SF} = 0.4895V/V $$

$$ A_d = A_{SF} - A_{CS} = .9856 $$

Using measurements from the voltages from the experiment the gain of the amplifier is calculated.

$$ A_{CS} = \frac{v_{o1}}{v_{in}} = \frac{.459V}{1.024V} = 0.4482V/V $$

$$ A_{SF} = \frac{v_{o2}}{v_{in}} = \frac{.462V}{1.024V} = 0.4512V/V $$

$$ A_d = A_{SF} - A_{CS} = .8994 $$

Calculating the gains of the circuit we can see the experimental setup has slightly less gain but has an error of $\epsilon = \frac{abs(1 - .8994)}{1} = 10%$ this is decent for the amplifier in a real lab with limited supply of resistors and mosfets.

# Procedure 5

INSERT PICTURE FROM GOOGLE DRIVE AND WRITE ABOUT IT

# Procedure 6

| Parameter | Design Prediction | Adjusted Predictions | Actual Measurements |
| :----:    | :----:            |       :----:         |    :----:           |
| R~2~|545$\Omega$ | 545$\Omega$ | 556$\Omega$ | 
| R~3~|667$\Omega$ | 667$\Omega$ | 659$\Omega$ | 
| R~4~|300$\Omega$ | 300$\Omega$ | 327$\Omega$ | 
| V~G2~ | 8.34V | 8.4288V | 8.3422V | 
| V~S2~ | 1.84V | 1.9238V | 2.3057V | 
| V~D2~ | 7.75V |  7.7199V | 8.0540V | 
| I~D~ | 3.38mA | 3.38mA | 3.46mA|

After connecting the signal generator to the circuit the following values were recorded.

$$ v_{in} = 0.992V $$

$$ v_{s2} = 0.03526V $$

$$ v_{d2} = 0.0784V $$

The gain of the circuit is calculated by $A_{CG} = \frac{v_{g1}}{v_{g2}} = \frac{7.8286V}{8.3422V} = 0.9384V/V$

PUT IN GRAPHS

WRITE CONCLUSION
