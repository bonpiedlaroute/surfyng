FROM ubuntu:18.04
ENV THRIFT_VERSION 0.10.0

RUN apt-get install -qqy automake bison flex g++ git libboost-all-dev libevent-dev libssl-dev libtool make pkg-config wget
RUN wget "http://apache.mirrors.spacedump.net/thrift/$THRIFT_VERSION/thrift-$THRIFT_VERSION.tar.gz"
RUN mkdir -p /home/ubuntu/software
RUN tar zxf "thrift-$THRIFT_VERSION.tar.gz" -C /home/ubuntu/software
RUN rm -f "thrift-$THRIFT_VERSION.tar.gz"
RUN cd "/home/ubuntu/software/thrift-$THRIFT_VERSION"
RUN ./configure \
	&& make \
	&& make install
RUN echo 'export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH' >> ~/.bashrc
RUN echo 'export PYTHONPATH=/home/ubuntu/software/thrift-0.10.0/lib/py/build/lib.linux-x86_64-2.7:$PYTHONPATH' >> ~/.bashrc
RUN cd ../
RUN git clone https://github.com/bonpiedlaroute/surfyn.git
RUN mkdir -p surfyn/sklearn/data
RUN mkdir sklearn_env
RUN apt-get install -qqy virtualenv
RUN virtualenv sklearn_env/
RUN cd sklearn_env/
RUN source bin/activate
RUN pip install scikit-learn==0.20.3
RUN pip install pandas==0.24.2
RUN pip install configparser

