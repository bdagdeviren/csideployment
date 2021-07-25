package main

import "C"
import (
	"context"
	"io/ioutil"
	metav1 "k8s.io/apimachinery/pkg/apis/meta/v1"
	"k8s.io/client-go/kubernetes"
	"k8s.io/client-go/rest"
	"net/http"
	"strings"
)

func CreateInClusterClient() (*kubernetes.Clientset,error) {
	config, err := rest.InClusterConfig()
	if err != nil {
		return nil,err
	}

	clientSet, err := kubernetes.NewForConfig(config)
	if err != nil {
		return nil,err
	}

	return clientSet,nil
}

//export get_pod_ips
func get_pod_ips(podName *C.char,podNamespace *C.char) (rc int,result *C.char,errStr *C.char) {
	clientSet,err := CreateInClusterClient()
	if err != nil {
		return -1, nil, C.CString(err.Error())
	}

	pods, err := clientSet.CoreV1().Pods(C.GoString(podNamespace)).List(context.TODO(), metav1.ListOptions{})
	if err != nil {
		return -1, nil, C.CString(err.Error())
	}

	ips := ""

	for _, pod := range pods.Items {
		if strings.Contains(pod.Name,C.GoString(podName)){
			ips+=pod.Status.PodIP + ","
		}
	}

	ips = strings.Trim(ips,",")

	return 0, C.CString(ips),nil
}

//export get_worker_ips
func get_worker_ips(ip *C.char,port *C.char) (rc int,result *C.char,errStr *C.char) {
	resp, err := http.Get("https://"+C.GoString(ip)+":"+C.GoString(port)+"/ip")
	if err != nil {
		return -1, nil, C.CString(err.Error())
	}

	body, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		return -1, nil, C.CString(err.Error())
	}

	return 0, C.CString(string(body)),nil
}

func main() {}
