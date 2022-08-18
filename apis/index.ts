import axios, { CancelToken } from "axios";

const request = axios.create();

const AlgorithmApi = {
  getCategories: () => request.get("/api/algorithms"),
  getAlgorithm: (categoryKey: string, algorithmKey: string, fileName: string) =>
    request.get(`/api/algorithms/${categoryKey}/${algorithmKey}/${fileName}`),
};

const TracerApi = {
  js: ({ code }: any, cancelToken: CancelToken) =>
    new Promise<any>((resolve, reject) => {
      const worker = new Worker("/tracers/js/worker.js");
      if (cancelToken) {
        cancelToken.promise.then((cancel) => {
          worker.terminate();
          reject(cancel);
        });
      }
      worker.onmessage = (e) => {
        worker.terminate();
        resolve(e.data);
      };
      worker.onerror = (error) => {
        worker.terminate();
        reject(error);
      };
      worker.postMessage(code);
    }),
};

export { AlgorithmApi, TracerApi };
