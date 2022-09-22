import pandas as pd
import matplotlib.pyplot as plt
import os


# cur_path = os.path.dirname(__file__)

class Process_Data:
    def __init__(self, path2):
        self.files = []
        self.path1 = path2
        self.firstIndex = 2
        self.steps = 10000000

        self.inner_total = 0
        self.moto_total = 0
        self.outer_total = 0

    def get_csv_file(self):
        for (root, dirs, file) in os.walk(self.path1):
            for f in file:
                if '.csv' in f:
                    # print(root)
                    real_path = root + '/' + f
                    # print(real_path)
                    # self.files.append(os.path.abspath(f))
                    self.files.append(real_path)
        # print(self.files)

    def sum_data(self):
        for fil in self.files:
            c_inner, c_moto, c_outer = self.get_data(fil)
            self.inner_total += c_inner
            self.moto_total += c_moto
            self.outer_total += c_outer
        # print(self.inner_total)

    def addlabels(self, x, y):
        for i in range(len(x)):
            plt.text(i, y[i], y[i], horizontalalignment='center', verticalalignment='center',
                     bbox=dict(facecolor='orange', alpha=0.4))

    def process_data(self):
        innerdis = self.inner_total / 13 * 0.038 * 3.1415
        outerdis = self.outer_total / 13 * 0.038 * 3.1415
        motodis = self.moto_total / 15 * 0.197 * 3.1415
        innerwheel = self.moto_total / 15 * 0.1499 * 3.1415

        innerpercent = (innerdis - innerwheel) / innerwheel
        if (innerpercent < 0):
            innerpercent = -innerpercent
        outerpercent = (outerdis - motodis) / motodis
        if (outerpercent < 0):
            outerpercent = -outerpercent
        return innerdis, outerdis, motodis, innerwheel, innerpercent, outerpercent

    def plt_data(self):
        innerdis, outerdis, motodis, innerwheel, innerpercent, outerpercent = self.process_data()
        dataItem = {"Inner": innerdis, "innerwheel": innerwheel, "Outer": outerdis, "outerwheel": motodis}
        items = list(dataItem.keys())
        dis = list(dataItem.values())
        fig = plt.figure(figsize=(10, 7))

        # fig= plt.figure((7,7))
        # creating the bar plot
        # colors=['#E0FF5A','#FFEB00',"orange"]
        plt.bar(items, dis, color='b', width=0.2)
        self.addlabels(items, dis)
        plt.xlabel("Total Mileage")
        plt.ylabel("No. of Total Mileage  m")

        plt.title("Outer encoder/Outer wheel distance percent: %.4f%%\n" % (
                outerpercent * 100) + "Inner encoder/Inner wheel distance percent: %.4f%%" % (
                          innerpercent * 100))

        plt.show()

    def get_data(self, file):
        with open(file, "r") as fd:
            data = pd.read_csv(fd)
        # print(data)
        try:
            inner = data.inner[self.firstIndex + self.steps] # 14400
            moto = data.moto[self.firstIndex + self.steps]
            outer = data.outer[self.firstIndex + self.steps]
        except Exception:
            # print(data)
            inner = data.inner[len(data.inner) - 2]  # 14400
            moto = data.moto[len(data.inner) - 2]
            outer = data.outer[len(data.inner) - 2]
        # print(inner)
        return inner, moto, outer


if __name__ == "__main__":
    path1 = "/home/yj/Documents/python/Test_total_mill"
    process_data = Process_Data(path1)

    process_data.get_csv_file()
    process_data.sum_data()
    process_data.plt_data()
