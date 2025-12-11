import { FileImage, Ruler, Palette, Calendar } from 'lucide-react';

interface InfoTabProps {
  imageInfo: {
    name: string;
    width: number;
    height: number;
    size: string;
    type: string;
    modified: string;
  } | null;
}

export function InfoTab({ imageInfo }: InfoTabProps) {
  if (!imageInfo) {
    return (
      <div className="p-4 flex items-center justify-center h-full text-purple-400/60">
        <p>No image loaded</p>
      </div>
    );
  }

  const infoItems = [
    { icon: FileImage, label: 'File Name', value: imageInfo.name },
    { icon: Ruler, label: 'Dimensions', value: `${imageInfo.width} × ${imageInfo.height} px` },
    { icon: Palette, label: 'File Type', value: imageInfo.type },
    { icon: FileImage, label: 'File Size', value: imageInfo.size },
    { icon: Calendar, label: 'Modified', value: imageInfo.modified },
  ];

  return (
    <div className="p-4 space-y-3">
      <h3 className="text-fuchsia-300">Image Information</h3>
      
      <div className="space-y-2">
        {infoItems.map((item, index) => (
          <div
            key={index}
            className="bg-purple-900/20 border border-purple-800/30 rounded-lg p-3 hover:border-fuchsia-500/30 transition-colors"
          >
            <div className="flex items-start gap-3">
              <item.icon className="w-4 h-4 text-fuchsia-400 mt-0.5" />
              <div className="flex-1 min-w-0">
                <label className="block text-purple-300 mb-1">{item.label}</label>
                <p className="text-purple-100 break-words">{item.value}</p>
              </div>
            </div>
          </div>
        ))}
      </div>

      <div className="mt-6 p-4 bg-gradient-to-br from-fuchsia-500/10 to-purple-500/10 border border-fuchsia-500/20 rounded-lg">
        <h4 className="text-sm text-fuchsia-300 mb-2">Quick Stats</h4>
        <div className="grid grid-cols-2 gap-3">
          <div>
            <label className="text-purple-400">Aspect Ratio</label>
            <p className="text-purple-100">
              {(imageInfo.width / imageInfo.height).toFixed(2)}:1
            </p>
          </div>
          <div>
            <label className="text-purple-400">Megapixels</label>
            <p className="text-purple-100">
              {((imageInfo.width * imageInfo.height) / 1000000).toFixed(2)} MP
            </p>
          </div>
        </div>
      </div>
    </div>
  );
}
