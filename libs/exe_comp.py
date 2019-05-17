import time


def main():
    t = time.time()

    name = "Kasun"
    lowerName = name.lower()
    print(lowerName)

    tt = (time.time() - t) * 1000*1000
    print("Time to execute = {}".format(tt))


if __name__ == "__main__":
    main()
