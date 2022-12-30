import cv2.cv2 as cv2
import time


if __name__ == '__main__':

    video_filename = str(int(time.time()))
    capture_height = 480
    capture_width = 640

    capture_height = 720
    capture_width = 1024
    fourcc = cv2.VideoWriter_fourcc(*'XVID')
    vw = cv2.VideoWriter(video_filename + ".avi", fourcc, 20, (capture_width, capture_height))

    cv2.namedWindow("SONY IMAX179", cv2.WINDOW_NORMAL)
    cv2.resizeWindow("SONY IMAX179", capture_width, capture_height)

    cap = cv2.VideoCapture(1)

    cap.set(cv2.CAP_PROP_FRAME_WIDTH, capture_width)
    cap.set(cv2.CAP_PROP_FRAME_HEIGHT, capture_height)

    while cap.isOpened():
        ret, frame = cap.read()
        if ret == True:
            cv2.imshow("SONY IMAX179", frame)
            vw.write(frame)
            key = cv2.waitKey(1)
            if (key & 0xFF == ord('q')):
                break
        else:
            break

    cap.release()
    vw.release()
    cv2.destroyAllWindows()
