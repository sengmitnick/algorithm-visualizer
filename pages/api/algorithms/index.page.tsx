// Next.js API route support: https://nextjs.org/docs/api-routes/introduction
import fs from "fs";
import path from "path";
import { promisify } from "util";
import type { NextApiRequest, NextApiResponse } from "next";
import { errorToJSON } from "utils";

async function generateTree(root: string) {
  var filesNameArr = [];
  let cur = 0;
  // 用个hash队列保存每个目录的深度
  var mapDeep: any = {};
  mapDeep[root] = 0;
  // 先遍历一遍给其建立深度索引
  async function getMap(dir: any, curIndex: any) {
    var files: any[] = await promisify(fs.readdir)(dir); //同步拿到文件目录下的所有文件名
    for (const file of files) {
      //var subPath = path.resolve(dir, file) //拼接为绝对路径
      var subPath = path.join(dir, file); //拼接为相对路径
      var stats = await promisify(fs.stat)(subPath); //拿到文件信息对象
      // 必须过滤掉node_modules文件夹
      if (file != "node_modules") {
        mapDeep[file] = curIndex + 1;
        if (stats.isDirectory()) {
          //判断是否为文件夹类型
          await getMap(subPath, mapDeep[file]); //递归读取文件夹
        }
      }
    }
  }
  await getMap(root, mapDeep[root]);
  async function readdirs(dir: string, folderName: any, myroot?: any) {
    var result: any = {
      //构造文件夹数据
      path: dir,
      key: dir.replace(root, ""),
      title: path.basename(dir),
      type: "directory",
      deep: mapDeep[folderName],
    };
    var files = await promisify(fs.readdir)(dir); //同步拿到文件目录下的所有文件名
    result.children = [];
    for (let index = 0; index < files.length; index++) {
      const file = files[index];
      //var subPath = path.resolve(dir, file) //拼接为绝对路径
      var subPath = path.join(dir, file); //拼接为相对路径
      var stats = await promisify(fs.stat)(subPath); //拿到文件信息对象
      if (stats.isDirectory()) {
        //判断是否为文件夹类型
        result.children[index] = await readdirs(subPath, file, file); //递归读取文件夹
      } else {
        result.children[index] = {
          //构造文件数据
          path: subPath,
          key: subPath.replace(root, ""),
          title: file,
          type: "file",
        };
      }
    }
    return result; //返回数据
  }
  filesNameArr.push(await readdirs(root, root));
  return filesNameArr;
}

export default async function handler(_: NextApiRequest, res: NextApiResponse) {
  try {
    const dir = path.resolve("algorithm");
    const tree = await generateTree(dir);
    res.status(200).json({ tree });
  } catch (error: any) {
    res.status(404).json({ error: errorToJSON(error) });
    return;
  }
}
