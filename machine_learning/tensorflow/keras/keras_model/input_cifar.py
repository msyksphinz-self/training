import cPickle
import numpy as np
import os

def unpickle(file):
    fo = open(file, 'rb')
    dict = cPickle.load(fo)
    fo.close()
    return dict

def conv_data2image(data):
    return np.rollaxis(data.reshape((3,32,32)),0,3)

def get_cifar10(folder):
    tr_data = np.empty((0,32*32*3))
    tr_labels = np.empty(1)
    '''
    32x32x3
    '''
    for i in range(1,6):
        fname = os.path.join(folder, "%s%d" % ("data_batch_", i))
        data_dict = unpickle(fname)
        if i == 1:
            tr_data = data_dict['data']
            tr_labels = data_dict['labels']
        else:
            tr_data = np.vstack((tr_data, data_dict['data']))
            tr_labels = np.hstack((tr_labels, data_dict['labels']))

    data_dict = unpickle(os.path.join(folder, 'test_batch'))
    te_data = data_dict['data']
    te_labels = np.array(data_dict['labels'])

    bm = unpickle(os.path.join(folder, 'batches.meta'))
    label_names = bm['label_names']
    return tr_data, tr_labels, te_data, te_labels, label_names

def get_cifar100(folder):
    train_fname = os.path.join(folder,'train')
    test_fname  = os.path.join(folder,'test')
    data_dict = unpickle(train_fname)
    train_data = data_dict['data']
    train_fine_labels = data_dict['fine_labels']
    train_coarse_labels = data_dict['coarse_labels']

    data_dict = unpickle(test_fname)
    test_data = data_dict['data']
    test_fine_labels = data_dict['fine_labels']
    test_coarse_labels = data_dict['coarse_labels']

    bm = unpickle(os.path.join(folder, 'meta'))
    clabel_names = bm['coarse_label_names']
    flabel_names = bm['fine_label_names']

    return train_data, np.array(train_coarse_labels), np.array(train_fine_labels), test_data, np.array(test_coarse_labels), np.array(test_fine_labels), clabel_names, flabel_names

if __name__ == '__main__':
    datapath = "./cifar-10-batches-py"
    # datapath2 = "./cifar-100-python"

    tr_data10, tr_labels10, te_data10, te_labels10, label_names10 = get_cifar10(datapath)
    # tr_data100, tr_clabels100, tr_flabels100, te_data100, te_clabels100, te_flabels100, clabel_names100, flabel_names100 = get_cifar100(datapath2)
    print("3 32 32")
    print("%f. " % (te_data10[0][0] / 10.0))
    for dim in range(3):
        for y in range(32):
            print("["),
            for x in range (32):
                print("%f. " % (int(te_data10[0][dim * 32 * 32 + y * 32 + x]) / 256.0)),
            print("]")
