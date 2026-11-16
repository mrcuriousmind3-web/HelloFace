from PIL import Image, ImageDraw, ImageFont
import os, subprocess
W,H=1280,720
BG=(13,23,37); PANEL=(24,39,56); MINT=(112,225,181); WHITE=(244,247,251); MUTED=(157,174,192)
def font(size,bold=False):
    path="C:/Windows/Fonts/segoeuib.ttf" if bold else "C:/Windows/Fonts/segoeui.ttf"
    try: return ImageFont.truetype(path,size)
    except: return ImageFont.load_default()
def slide(title,subtitle,step,index):
    im=Image.new("RGB",(W,H),BG); d=ImageDraw.Draw(im)
    d.rounded_rectangle((58,48,112,102),radius=16,outline=MINT,width=5)
    d.ellipse((76,58,96,91),outline=WHITE,width=4)
    d.text((135,54),"HelloFace",font=font(28,True),fill=WHITE)
    d.text((60,170),title,font=font(48,True),fill=WHITE)
    d.text((64,245),subtitle,font=font(22),fill=MUTED)
    d.rounded_rectangle((60,340,1220,540),radius=26,fill=PANEL,outline=(48,67,86),width=2)
    d.text((100,405),step,font=font(28,True),fill=MINT)
    d.text((60,625),"EXPERIMENTAL PREVIEW  •  RGB FACE MATCHING  •  WINDOWS 10/11",font=font(16,True),fill=MUTED)
    d.text((1090,625),"HELLOFACE",font=font(16,True),fill=MINT)
    im.save(f"dist/frame{index:02d}.png")
slides=[
("Face matching, locally","An open-source Windows research prototype.","HELLOFACE  /  LOCAL-FIRST WINDOWS PREVIEW"),
("Four profile slots","Manage up to four local profile slots.","SLOT 01     SLOT 02     SLOT 03     SLOT 04"),
("A careful vision pipeline","Capture, detect, align, embed and compare.","CAMERA  →  LANDMARKS  →  512-D EMBEDDING"),
("Liveness has limits","Texture, motion and challenge checks help — RGB is not IR or depth.","PRESENTATION ATTACK DETECTION  /  NOT PERFECT"),
("Security by separation","Keep inference outside LogonUI; preserve fallback sign-in.","BOUNDED IPC  •  TPM KEY DESIGN  •  PIN FALLBACK"),
("Early scaffold — not sign-in ready","The protocol, documentation and preview UI are the current milestone.","CAMERA / ONNX / VAULT / CREDENTIAL PROVIDER IN DEVELOPMENT")
]
os.makedirs("dist",exist_ok=True)
for i,s in enumerate(slides): slide(*s,i)
concat="dist/frames.txt"
with open(concat,"w",encoding="utf-8") as f:
    for i in range(len(slides)): f.write(f"file 'frame{i:02d}.png'\nduration 3\n")
    f.write(f"file 'frame{len(slides)-1:02d}.png'\n")
subprocess.run(["ffmpeg","-y","-f","concat","-safe","0","-i",concat,"-vf","fps=30,format=yuv420p","-c:v","libx264","-movflags","+faststart","dist/HelloFace-Explainer.mp4"],check=True)
