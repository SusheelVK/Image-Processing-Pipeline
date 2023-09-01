from PIL import Image
import numpy as np
im = Image.open("lighthouse_RAW_noisy_sigma0.01.png")
pixels = np.array((im.getdata()))
np.savetxt("lighthouse_bayer.txt", pixels, fmt="%u,")
