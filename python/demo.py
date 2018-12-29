import cv2
import time

start = int(round(time.time() * 1000))
 
# Load a model imported from Tensorflow
tensorflowNet = cv2.dnn.readNetFromTensorflow('frozen_inference_graph.pb', 'graph.pbtxt')
 
# Input image
img = cv2.imread('demo.png')
rows, cols, channels = img.shape
 
# Use the given image as input, which needs to be blob(s).
tensorflowNet.setInput(cv2.dnn.blobFromImage(img, size=(1000, 1000), swapRB=True, crop=False))
#tensorflowNet.setPreferableTarget(cv2.dnn.DNN_TARGET_OPENCL)
 
# Runs a forward pass to compute the net output
networkOutput = tensorflowNet.forward()
 
# Loop on the outputs
for detection in networkOutput[0,0]:
    
    score = float(detection[2])
    if score > 0.5:
    	
        left = detection[3] * cols
        top = detection[4] * rows
        right = detection[5] * cols
        bottom = detection[6] * rows
 
        #draw a red rectangle around detected objects
        cv2.rectangle(img, (int(left), int(top)), (int(right), int(bottom)), (0, 0, 255), thickness=2)

end = int(round(time.time() * 1000))
print('time:',end - start)
cv2.imwrite("result.png",img)
 
# Show the image with a rectagle surrounding the detected objects 
#cv2.imshow('Image', img)
#cv2.waitKey()
#cv2.destroyAllWindows()
