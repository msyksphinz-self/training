本チュートリアルでは、TensorFlow Servingがどのようにして訓練されたモデルを使って、サーバを構築し、モデルを使ったアプリケーションを実行するかを説明するためのチュートリアルである。
本チュートリアルにおいて説明に使用するモデルは非常にシンプルなもので、TensorFlowのモデルであり、個々のリクエスト(バッチリクエストではない)を処理し、全体のエラー率を計算するものである。
既にTensorFlow Servingについて知識があり、バッチ処理リクエストのような、より複雑なサーバモデルを構築したい場合や、動的にアップデートされる新しいTensorFolwモデルを見つけたり、構築したい場合には、TensorFlow Serving Advanced Tutorialを参照されたい。

本チュートリアルでは、TensorFlowのチュートリアルにおいて「手書き文字認識(MNIST Data)」で利用したSoftmaxのリグレッションモデルを利用する。
もし読者がTensorFlow MNISTについて知らなかったら、MNIST For ML Beginnersのチュートリアルを参照して欲しい。

本チュートリアルのコードは2種類から構成される。Pythonファイル(mnist_export.py)は、学習を行い、モデルをエクスポートする。
C++ファイル(mnist_inference.cc)はエクスポートされたモデルをロードし、gRPCサービスを使ってサービスを実行する。

まず始める前に「事前準備」を完了させておいて欲しい。

# TensorFlowモデルの学習とエクスポート

`mnist_export.py`を見れば分かるように、MNIST For ML Beginnersチュートリアルと同様の方法で学習を行っている。
TensorFlowセッション(sess)においてTensorFlow Graphが実行され、入力テンソル(画像)がxとして出力され、それに対するに対する出力テンソル(Softmax値)がyとして出力される。

次に、TensorFlow ServingのExporterモジュールを使って、モデルをエクスポートする。
Exporterは学習したモデルの「スナップショット」をストレージに保存し、後の推論時にロードすることができる。

```python
from tensorflow_serving.session_bundle import exporter
...
export_path = sys.argv[-1]
print 'Exporting trained model to', export_path
saver = tf.train.Saver(sharded=True)
model_exporter = exporter.Exporter(saver)
signature = exporter.classification_signature(input_tensor=x, scores_tensor=y)
model_exporter.init(sess.graph.as_graph_def(),
                    default_graph_signature=signature)
model_exporter.export(export_path, tf.constant(FLAGS.export_version), sess)
```

`Exporter.__init__`は`tensorflow.train.Saver`を引数に取る。このときに必要なのは、Serverがshared=Trueであることのみである。
`saver`はグラフの値をモデルのエクスポートのためにシリアライズし、モデルを正常に復元する。
`Saver`において`variable_list`が指定されていないのは、グラフの全ての変数をエクスポートするためである。
より複雑なグラフでは、後に推論に利用するための変数のみを選択することもできる。

Exporter.init()は以下の引数を取る:

* `sess.graph.as_graph_def()`はグラフの`protobuf`である。`export`は`protobuf`モデル`export`にシリアライズし、これによりTensorFlowグラフを正常に復元できるようになる。
* `default_graph_signature=signature`は`model`のエクスポートの「シグニチャ」を指定する。
シグニチャは、どのようなタイプのモデルと、入出力のテンソルをエクスポートするかを指定し、推論を実行する際にバインドする。
この場合には、`exporter.classification_signature`を使用することで、このモデルが分類モデルであることを指定できる。
** input_tensor=xは入力テンソルのバインドである。
** scores_tensor=yは出力テンソルのバインドである。
** 典型的に、classes_tensor=Noneもオーバライドするべきであり、テンソルのバインドを指定するものである。
例えば、分類モデルにおいて、トップ10のおすすめビデオを指定する場合には、出力はビデオ(分類)と各ビデオのスコアを出力する。
しかし今回の場合には、モデルは常に0-9までにマッチングさせるSoftmaxのスコアを出力する。従って、`classes_tensor`は必要ない。

`Exporter.export`()は以下の引数を取る。

* `export_path`はエクスポートするディレクトリを指定する。ディレクトリが存在しなければ作成する。
* `tf.constant(FLAGS.export_version)`はモデルの「バージョン」を指定する。同一モデルにおいて、新しいバージョンをエクスポートする時は、より大きな数値を指定すべきである。各バージョンは、与えられたパスにおいて、異なるサブディレクトリに格納される。
* sessは訓練させ、エクスポートするモデルが含まれるTensorFlowセッションを指定する。

では、実行してみよう！

既にディレクトリが存在しているならば、消去する。

```sh
$>rm -rf /tmp/mnist_model
```

```sh
$>bazel build //tensorflow_serving/example:mnist_export
$>bazel-bin/tensorflow_serving/example/mnist_export /tmp/mnist_model
```

では、エクスポートしたディレクトリを見てみよう。

```sh
$>ls /tmp/mnist_model
00000001
```

上記で述べたように、バージョンに応じたサブディレクトリが作成されている。
`tf.constant(FLAGS.export_version)`でモデルのバージョンを指定しており、`FLAGS.export_version`のデフォルト値は1である。
従って、該当するサブディレクトリである00000001が作成された。

```sh
$>ls /tmp/mnist_model/00000001
export.meta export-00000-of-00001
```

各バージョンのサブディレクトリには、以下のファイルが入っている。

* `export.meta`はモデルのシリアライズ化された`TensorFlow::MetaGraphDef`である。
これにはモデルのグラフ定義と、シグニチャのようなモデルのメタデータが含まれている。

* `export-?????-of-?????`はグラフのシリアライズされた変数が含まれたファイルである。

これらにより、TensorFlowのモデルはエクスポートされ、別のプログラムによってロードされる準備が整った！

# TensorFlowのエクスポートモデルをロードする

TensorFlowのモデルをロードするためのC++のコードはとてもシンプルだ(mnist_inference.ccを参照のこと)。

```cpp
int main(int argc, char** argv) {
  ...

  tensorflow::SessionOptions session_options;
  std::unique_ptr<SessionBundle> bundle(new SessionBundle);
  const tensorflow::Status status =
      tensorflow::serving::LoadSessionBundleFromPath(session_options,
                                                     bundle_path, bundle.get());
  ...

  RunServer(FLAGS_port, std::move(bundle));

  return 0;
}
```

TensorFlow Servingを使用するために、`SessionBundle`コンポーネントを使用している。
`LoadSessionBundleFromPath()`はエクスポートしたTensorFlowモデルのパスを指定し、`SessionBundle`オブジェクトを作成する。このモデルを利用して、実際にサービスを提供するという訳だ。
典型的に、モデルがロードされた時に、`SessionOptions`のデフォルト値が与えられる。

`SessionBundle`の定義を、`session_bundle.h`で見てみよう。

```cpp
struct SessionBundle {
  std::unique_ptr<tensorflow::Session> session;
  tensorflow::MetaGraphDef meta_graph_def;
};
```

`session`は、予想するに、TensorFlowのセッションであり、必要な変数が保存されたグラフを含んでいるだろう。
言い変えれば、学習したモデルは`session`内に保存されており、既に実行される準備が整っていると言える！

この実行モデルに対して、入力と出力に正しいテンソルをバインドし、`session->run()`を実行すれば良い。
しかし、どのテンソルがどの入力値とバインドされるかという情報は、どのようにして知れば良いのだろうか？
おそらく読者が予想したように、`meta_graph_def`を使用するというのが答えだ。

`tensorflow::MetaGraphDef`は上記の`export.meta`ファイルを逆シリアライズした`protobuf`である(`meta_graph.proto`を参照のこと)。
TensorFlowモデルからエクスポートした、必要な記述およびメタデータの全てを、拡張可能な`collection_def`に追加する。
特に、`Signatures`(`manifest.proto`を参照のこと)は使用するテンソルを含んでおり、これを含んでいる。

```cpp
// Signatures of model export.
message Signatures {
  // Default signature of the graph.
  Signature default_signature = 1;

  // Named signatures of the graph.
  map<string, Signature> named_signatures = 2;
};
```

`export`において、どのようにしてモデルのシグニチャを指定したかについて思い出そう。
必要な情報は、エンコードされた状態で入手できる。

```cpp
message ClassificationSignature {
  TensorBinding input = 1;
  TensorBinding classes = 2;
  TensorBinding scores = 3;
};
```

TensorBindingには、`session->run()`時に使用できるテンソルの名前が含まれている。
これにより、`SessionBundle`により与えられた実行モデルを実行できるようになる！

# モデル実行サービスを立ち上げる

`mnist_inference.cc`で見たように、`main`関数内の`RunServer`はgRPCサーバにより単一のClassify()APIを立ち上げる。
この手法の実装は、実行時のリクエストは以下のように処理される単純明快なものである。

1. 入力値のチェック - サーバーはMNISTフォーマットの画像がモデルに入力されたかをチェックする
2. protobufの入力から、出力テンソルのプレースホルダに変換する
3. モデルを実行する - `MnistServiceImpl`コンストラクタは`GetClassificationSignature`を使用し、`meta_graph_def`からモデルのシグニチャを出力し、その分類シグニチャが正しいものであるかをチェックする。
抽出されたシグニチャを用い、サーバは入力と出力テンソルを正しくバインドし、セッションを実行する。

```cpp
  const tensorflow::Status status =
      bundle_->session->Run(,
                            {signature_.scores().tensor_name()}, {},
                            &outputs);
```

4. 実行モデルの出力テンソルをprotobufの出力に変換する。

これを実行するためには、

```sh
$>bazel build //tensorflow_serving/example:mnist_inference
$>bazel-bin/tensorflow_serving/example/mnist_inference --port=9000 /tmp/mnist_model/00000001
```

# サーバのテスト

今回チュートリアルとして使用した`mnist_client`ユーティリティを、サーバのテストに利用できる。
クライアントはMNISTのテストデータをダウンロードし、サーバにこれらをリクエストとして送信し、モデルのエラー率を計算する。

これを実行するためには、

```sh
$>bazel build //tensorflow_serving/example:mnist_client
$>bazel-bin/tensorflow_serving/example/mnist_client --num_tests=1000 --server=localhost:9000
...
Inference error rate: 10.5%
```

最初の1000個のテスト画像において、おおよそ91%の精度で、学習したSoftmaxのモデルが実行され、10.5のエラー率となった。
これにより、サーバが学習したモデルを正常にロードし、実行されたことが確認できた！

