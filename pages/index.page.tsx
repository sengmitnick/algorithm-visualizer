import fs from "fs";
import path from "path";
import { promisify } from "util";
import { useRouter } from "next/router";
import { Tree, Typography } from "antd";
import { GetServerSideProps } from "next";
import { extension } from "common/util";
import { useRef } from "react";
import { useSize } from "ahooks";

const { DirectoryTree } = Tree;
const { Title, Paragraph, Text } = Typography;

function generateTreeData(data: any[]) {
  return data.map((item) => {
    if (item.children) {
      item.children = generateTreeData(
        item.children.filter((file: any) => {
          if (file.type === "file") {
            const ext = extension(file.title);
            return ["cpp", "js"].includes(ext);
          }
          return true;
        })
      );
    }
    return item;
  });
}

async function generateTree(root: string) {
  const filesNameArr = [];
  let cur = 0;
  // 用个hash队列保存每个目录的深度
  const mapDeep: any = {};
  mapDeep[root] = 0;
  // 先遍历一遍给其建立深度索引
  async function getMap(dir: any, curIndex: any) {
    const files: any[] = await promisify(fs.readdir)(dir); //同步拿到文件目录下的所有文件名
    for (const file of files) {
      //const subPath = path.resolve(dir, file) //拼接为绝对路径
      const subPath = path.join(dir, file); //拼接为相对路径
      const stats = await promisify(fs.stat)(subPath); //拿到文件信息对象
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
    const result: any = {
      //构造文件夹数据
      path: dir,
      key: dir.replace(root, ""),
      title: path.basename(dir),
      type: "directory",
      deep: mapDeep[folderName],
    };
    //同步拿到文件目录下的所有文件名
    const files = (await promisify(fs.readdir)(dir)).filter(
      (file) => ![".images", ".validate"].includes(file)
    );
    result.children = [];
    for (let index = 0; index < files.length; index++) {
      const file = files[index];
      //const subPath = path.resolve(dir, file) //拼接为绝对路径
      const subPath = path.join(dir, file); //拼接为相对路径
      const stats = await promisify(fs.stat)(subPath); //拿到文件信息对象
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
    if (!result.children.length) {
      delete result.children;
    }
    return result; //返回数据
  }
  filesNameArr.push(await readdirs(root, root));
  return generateTreeData(filesNameArr);
}

export const getServerSideProps: GetServerSideProps = async function (context) {
  const dir = path.resolve("algorithms");
  const treeData = await generateTree(dir);
  return {
    props: { ready: true, treeData }, // will be passed to the page component as props
  };
};

const Home = ({ treeData }: any) => {
  const router = useRouter();
  const ref = useRef<HTMLDivElement>(null);
  const size = useSize(ref);

  return (
    <div className="w-full h-full flex flex-col pt-20 px-10 pb-3">
      <Typography>
        <Title>算法可视化</Title>
        <Paragraph>
          点击下面某一个文件进入可视化界面，在编辑器修改该文件然后重新点击
          <Text code>build 按钮</Text>即可查看效果
        </Paragraph>
      </Typography>
      <div ref={ref} className="w-full flex-1">
        {!!size?.height && (
          <DirectoryTree
            height={size?.height}
            itemHeight={28}
            className="w-full"
            defaultExpandAll
            treeData={treeData}
            onSelect={(_, { node }) => {
              const { key, type } = node as any;
              if (type === "file") {
                router.push(key);
                window.parent.postMessage(
                  {
                    isOpenFile: true,
                    path: "algorithm-visualizer/algorithms" + key,
                  },
                  "*"
                );
              }
            }}
          />
        )}
      </div>
    </div>
  );
};

export default Home;
