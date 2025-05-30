from PIL import Image
import numpy as np
from scipy.spatial import KDTree

def extract_vga_palette(palette_image_path):
    img = Image.open(palette_image_path).convert("RGB")
    grid_size = 16
    cell_size = img.width // grid_size

    palette = []
    for row in range(grid_size):
        for col in range(grid_size):
            x = col * cell_size + cell_size // 2
            y = row * cell_size + cell_size // 2
            color = img.getpixel((x, y))
            palette.append(color)
    return np.array(palette)  # return as NumPy array for speed

def convert_image_to_vga_binary(input_image_path, palette, output_bin_path):
    # Build KD-tree for fast nearest-neighbor search
    tree = KDTree(palette)

    img = Image.open(input_image_path).convert("RGB")
    width, height = img.size

    # Center the image horizontally in a 320×200 frame
    canvas = Image.new("RGB", (320, 200), color=(0, 0, 0))
    paste_x = (320 - width) // 2
    canvas.paste(img, (paste_x, 0))

    pixels = np.array(canvas).reshape(-1, 3)  # flatten to (64000, 3)
    _, indices = tree.query(pixels)

    with open(output_bin_path, "wb") as f:
        byte_data = indices.astype(np.uint8).tobytes()
        f.write(byte_data)

    print(f"✅ Done! Saved to '{output_bin_path}' ({len(indices)} bytes)")

# === USAGE ===
vga_palette = extract_vga_palette("vga_palette.png")
convert_image_to_vga_binary("pal_200h.png", vga_palette, "VGATEST.BIN")