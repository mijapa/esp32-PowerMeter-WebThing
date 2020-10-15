# Webthing Energy Monitor on ESP32 with current sensor

## burden resisotor selection
https://bogdan.nimblex.net/diy/2017/04/19/apartment-energy-monitor.html

multiply current with √2 to get peak values: 50A × 1.414 = 70,7A
sensor is rated 50mA on the secondary for 100A on the primary so we have 100 x 1000 / 50 = 2000 turns
the peak current we can expect on the secondary will be: 70,7A / 2000 = 0.03535A
burden resistor should be chosen so that voltage doesn’t go over half of 3.3V: 1,65V / 0,03535A =~ 46,67Ω

we have 50Ω resistor then the current would be:

1,65V / 50Ω = 0,033A on secondary

0,033A * 2000 = 66A peak on primary

66A / 1,414 =~ 42,43A

https://tyler.anairo.com/projects/open-energy-monitor-calculator

## calibration
https://learn.openenergymonitor.org/electricity-monitoring/ctac/calibration

CT Ratio / Burden resistance = (100A / 0.05A) / 50 Ohms = 40

## optimisation
### signal filtering
https://www.casler.org/wordpress/low-current-measurement-performance-of-the-sct013/

filter capacitor, ranging from 0.01uF to 1.0uF

### stabilization of the reference voltage
why do I need an alternative path for the alternating current?

https://electronics.stackexchange.com/questions/215306/capacitor-for-current-transformer-measurement-circuit

Without the capacitor and driving a perfect ADC it will theoretically work.
However, when a capacitor is not used, the source impedance of the CT is made much bigger due to the resistances of the potential divider.
Again this isn't a problem until you start to look at the ADC spec for such devices as PICs and MCUs on arduinos.
Typically, the ADC on a device like this won't like the source impedance of the signal it is measuring to be much more than a couple of kohms and if your potential divider is 2x 10k resistors then the net source impedance will be 5k + a few ohms for the burden.
So putting a capacitor across the lower resistor in the potential divider means you have masterminded a way to significantly reduce the source impedance without busting-up the DC offset voltage.
10 uF at 50 Hz has an impedance of 318 ohms and clearly this is low enough for most MCUs with in-built ADCs.
