import cv2
import tensorflow as tf

CATEGORIES = ["glass", "aluminum"]

def prepare(filepath):
	IMG_SIZE = 100
	img_array = cv2.imread(filepath,cv2.IMREAD_GRAYSCALE)
	new_array = cv2.resize(img_array,(IMG_SIZE,IMG_SIZE))
	return new_array.reshape(-1,IMG_SIZE,IMG_SIZE,1)

model = tf.keras.models.load_model("Recycle-CNN.model")

prediction = model.predict([prepare("pepper.jpg")])
print(prediction)
print(CATEGORIES[int(prediction[0][0])])
