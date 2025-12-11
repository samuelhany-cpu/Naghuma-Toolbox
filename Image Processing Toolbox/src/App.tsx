import { useState, useRef, useEffect } from "react";
import {
  Layers,
  Sliders,
  BarChart3,
  Info,
  RotateCcw,
  Download,
  Upload,
  Sparkles,
} from "lucide-react";
import { TabButton } from "./components/TabButton";
import {
  ProcessesTab,
  ImageProcesses,
} from "./components/ProcessesTab";
import { LayersTab } from "./components/LayersTab";
import { HistogramTab } from "./components/HistogramTab";
import { InfoTab } from "./components/InfoTab";

type TabType = "layers" | "processes" | "histogram" | "info";

interface Layer {
  id: string;
  name: string;
  visible: boolean;
  opacity: number;
}

export default function App() {
  const [activeTab, setActiveTab] =
    useState<TabType>("processes");
  const [originalImage, setOriginalImage] = useState<
    string | null
  >(null);
  const [processedImage, setProcessedImage] = useState<
    string | null
  >(null);
  const [imageData, setImageData] = useState<ImageData | null>(
    null,
  );
  const [imageInfo, setImageInfo] = useState<any>(null);

  const [processes, setProcesses] = useState<ImageProcesses>({
    brightness: 0,
    contrast: 0,
    saturation: 0,
    blur: 0,
    hue: 0,
    sharpness: 0,
  });

  const [layers, setLayers] = useState<Layer[]>([
    { id: "1", name: "Original", visible: true, opacity: 100 },
    {
      id: "2",
      name: "Adjustments",
      visible: true,
      opacity: 100,
    },
  ]);

  const canvasRef = useRef<HTMLCanvasElement>(null);
  const fileInputRef = useRef<HTMLInputElement>(null);

  // Load default image on mount
  useEffect(() => {
    loadImageFromUrl(
      "https://images.unsplash.com/photo-1732808460864-b8e5eb489a52?crop=entropy&cs=tinysrgb&fit=max&fm=jpg&ixid=M3w3Nzg4Nzd8MHwxfHNlYXJjaHwxfHxuYXR1cmUlMjBsYW5kc2NhcGUlMjBzdW5zZXR8ZW58MXx8fHwxNzY1MTIwNjc0fDA&ixlib=rb-4.1.0&q=80&w=1080&utm_source=figma&utm_medium=referral",
    );
  }, []);

  const loadImageFromUrl = (url: string) => {
    const img = new Image();
    img.crossOrigin = "anonymous";
    img.onload = () => {
      setOriginalImage(url);
      setImageInfo({
        name: "sample-image.jpg",
        width: img.width,
        height: img.height,
        size: "2.4 MB",
        type: "JPEG",
        modified: new Date().toLocaleDateString(),
      });
    };
    img.src = url;
  };

  useEffect(() => {
    if (originalImage) {
      applyProcesses();
    }
  }, [processes, originalImage, layers]);

  const applyProcesses = () => {
    const img = new Image();
    img.crossOrigin = "anonymous";
    img.onload = () => {
      const canvas = canvasRef.current;
      if (!canvas) return;

      const ctx = canvas.getContext("2d");
      if (!ctx) return;

      // Set canvas size to match image
      canvas.width = img.width;
      canvas.height = img.height;

      // Apply layer opacity for original
      const originalLayer = layers.find((l) => l.id === "1");
      if (originalLayer?.visible) {
        ctx.globalAlpha = originalLayer.opacity / 100;
        ctx.drawImage(img, 0, 0);
      }

      // Apply filters
      const adjustmentsLayer = layers.find((l) => l.id === "2");
      if (adjustmentsLayer?.visible) {
        ctx.globalAlpha = adjustmentsLayer.opacity / 100;

        const filters = [];
        if (processes.brightness !== 0) {
          filters.push(
            `brightness(${100 + processes.brightness}%)`,
          );
        }
        if (processes.contrast !== 0) {
          filters.push(
            `contrast(${100 + processes.contrast}%)`,
          );
        }
        if (processes.saturation !== 0) {
          filters.push(
            `saturate(${100 + processes.saturation}%)`,
          );
        }
        if (processes.blur > 0) {
          filters.push(`blur(${processes.blur}px)`);
        }
        if (processes.hue !== 0) {
          filters.push(`hue-rotate(${processes.hue}deg)`);
        }

        ctx.filter =
          filters.length > 0 ? filters.join(" ") : "none";
        ctx.drawImage(img, 0, 0);
      }

      // Get image data for histogram
      ctx.globalAlpha = 1;
      ctx.filter = "none";
      const data = ctx.getImageData(
        0,
        0,
        canvas.width,
        canvas.height,
      );
      setImageData(data);

      // Convert canvas to data URL
      setProcessedImage(canvas.toDataURL("image/png"));
    };
    img.src = originalImage!;
  };

  const handleFileUpload = (
    e: React.ChangeEvent<HTMLInputElement>,
  ) => {
    const file = e.target.files?.[0];
    if (!file) return;

    const reader = new FileReader();
    reader.onload = (event) => {
      const url = event.target?.result as string;
      setOriginalImage(url);

      const img = new Image();
      img.onload = () => {
        setImageInfo({
          name: file.name,
          width: img.width,
          height: img.height,
          size: `${(file.size / 1024 / 1024).toFixed(2)} MB`,
          type: file.type.split("/")[1].toUpperCase(),
          modified: new Date(
            file.lastModified,
          ).toLocaleDateString(),
        });
      };
      img.src = url;
    };
    reader.readAsDataURL(file);
  };

  const handleReset = () => {
    setProcesses({
      brightness: 0,
      contrast: 0,
      saturation: 0,
      blur: 0,
      hue: 0,
      sharpness: 0,
    });
    setLayers([
      {
        id: "1",
        name: "Original",
        visible: true,
        opacity: 100,
      },
      {
        id: "2",
        name: "Adjustments",
        visible: true,
        opacity: 100,
      },
    ]);
  };

  const handleSave = () => {
    if (!processedImage) return;

    const link = document.createElement("a");
    link.download = "processed-image.png";
    link.href = processedImage;
    link.click();
  };

  const handleLayerToggle = (id: string) => {
    setLayers(
      layers.map((layer) =>
        layer.id === id
          ? { ...layer, visible: !layer.visible }
          : layer,
      ),
    );
  };

  const handleOpacityChange = (id: string, opacity: number) => {
    setLayers(
      layers.map((layer) =>
        layer.id === id ? { ...layer, opacity } : layer,
      ),
    );
  };

  return (
    <div className="min-h-screen bg-gradient-to-br from-purple-950 via-fuchsia-950 to-purple-900 flex flex-col">
      {/* Header */}
      <header className="bg-purple-950/50 backdrop-blur-sm border-b border-purple-800/30">
        <div className="px-6 py-4 flex items-center justify-between">
          <div className="flex items-center gap-3">
            <div className="w-10 h-10 bg-gradient-to-br from-fuchsia-400 to-purple-500 rounded-lg flex items-center justify-center">
              <Sparkles className="w-6 h-6 text-white" />
            </div>
            <div>
              <h1 className="text-purple-100">
                Image Processor
              </h1>
              <p className="text-xs text-purple-400">
                Professional Image Editor
              </p>
            </div>
          </div>

          <div className="flex items-center gap-3">
            <button
              onClick={() => fileInputRef.current?.click()}
              className="flex items-center gap-2 px-4 py-2 bg-purple-800/50 hover:bg-purple-700/50 text-purple-100 rounded-lg transition-colors border border-purple-700/50"
            >
              <Upload className="w-4 h-4" />
              Upload
            </button>
            <button
              onClick={handleReset}
              className="flex items-center gap-2 px-4 py-2 bg-rose-500/20 hover:bg-rose-500/30 text-rose-300 rounded-lg transition-colors border border-rose-500/30"
            >
              <RotateCcw className="w-4 h-4" />
              Reset
            </button>
            <button
              onClick={handleSave}
              className="flex items-center gap-2 px-4 py-2 bg-gradient-to-r from-fuchsia-500 to-purple-500 hover:from-fuchsia-600 hover:to-purple-600 text-white rounded-lg transition-all shadow-lg shadow-fuchsia-500/20"
            >
              <Download className="w-4 h-4" />
              Save
            </button>
          </div>
        </div>
      </header>

      <div className="flex-1 flex overflow-hidden">
        {/* Main Canvas Area */}
        <div className="flex-1 p-6 overflow-auto">
          <div className="grid grid-cols-2 gap-6 max-w-7xl mx-auto">
            {/* Original Image */}
            <div className="space-y-2">
              <label className="text-purple-300">
                Original
              </label>
              <div className="bg-purple-900/20 border-2 border-purple-800/30 rounded-xl overflow-hidden aspect-video">
                {originalImage ? (
                  <img
                    src={originalImage}
                    alt="Original"
                    className="w-full h-full object-contain"
                  />
                ) : (
                  <div className="w-full h-full flex items-center justify-center text-purple-400/60">
                    No image loaded
                  </div>
                )}
              </div>
            </div>

            {/* Processed Image */}
            <div className="space-y-2">
              <label className="text-fuchsia-300">
                Processed
              </label>
              <div className="bg-purple-900/20 border-2 border-fuchsia-500/30 rounded-xl overflow-hidden aspect-video shadow-lg shadow-fuchsia-500/10">
                {processedImage ? (
                  <img
                    src={processedImage}
                    alt="Processed"
                    className="w-full h-full object-contain"
                  />
                ) : (
                  <div className="w-full h-full flex items-center justify-center text-purple-400/60">
                    No processed image
                  </div>
                )}
              </div>
            </div>
          </div>
        </div>

        {/* Right Sidebar */}
        <div className="w-80 bg-purple-950/60 backdrop-blur-sm border-l border-purple-800/30 flex flex-col">
          {/* Tabs */}
          <div className="grid grid-cols-4 border-b border-purple-800/30">
            <TabButton
              active={activeTab === "layers"}
              onClick={() => setActiveTab("layers")}
              icon={<Layers className="w-5 h-5" />}
              label="Layers"
            />
            <TabButton
              active={activeTab === "processes"}
              onClick={() => setActiveTab("processes")}
              icon={<Sliders className="w-5 h-5" />}
              label="Process"
            />
            <TabButton
              active={activeTab === "histogram"}
              onClick={() => setActiveTab("histogram")}
              icon={<BarChart3 className="w-5 h-5" />}
              label="Histogram"
            />
            <TabButton
              active={activeTab === "info"}
              onClick={() => setActiveTab("info")}
              icon={<Info className="w-5 h-5" />}
              label="Info"
            />
          </div>

          {/* Tab Content */}
          <div className="flex-1 overflow-y-auto">
            {activeTab === "layers" && (
              <LayersTab
                layers={layers}
                onLayerToggle={handleLayerToggle}
                onOpacityChange={handleOpacityChange}
              />
            )}
            {activeTab === "processes" && (
              <ProcessesTab
                processes={processes}
                onProcessChange={setProcesses}
              />
            )}
            {activeTab === "histogram" && (
              <HistogramTab imageData={imageData} />
            )}
            {activeTab === "info" && (
              <InfoTab imageInfo={imageInfo} />
            )}
          </div>
        </div>
      </div>

      {/* Hidden canvas for image processing */}
      <canvas ref={canvasRef} className="hidden" />

      {/* Hidden file input */}
      <input
        ref={fileInputRef}
        type="file"
        accept="image/*"
        onChange={handleFileUpload}
        className="hidden"
      />
    </div>
  );
}