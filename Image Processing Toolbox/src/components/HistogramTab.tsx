import { BarChart, Bar, XAxis, YAxis, ResponsiveContainer, Cell } from 'recharts';
import { useEffect, useState } from 'react';

interface HistogramTabProps {
  imageData: ImageData | null;
}

export function HistogramTab({ imageData }: HistogramTabProps) {
  const [histogramData, setHistogramData] = useState<{ value: number; red: number; green: number; blue: number }[]>([]);

  useEffect(() => {
    if (!imageData) return;

    const red = new Array(256).fill(0);
    const green = new Array(256).fill(0);
    const blue = new Array(256).fill(0);

    for (let i = 0; i < imageData.data.length; i += 4) {
      red[imageData.data[i]]++;
      green[imageData.data[i + 1]]++;
      blue[imageData.data[i + 2]]++;
    }

    // Normalize the data
    const maxValue = Math.max(...red, ...green, ...blue);
    const data = [];
    
    // Sample every 4th value to reduce data points
    for (let i = 0; i < 256; i += 4) {
      data.push({
        value: i,
        red: (red[i] / maxValue) * 100,
        green: (green[i] / maxValue) * 100,
        blue: (blue[i] / maxValue) * 100,
      });
    }

    setHistogramData(data);
  }, [imageData]);

  return (
    <div className="p-4 space-y-4">
      <h3 className="text-fuchsia-300">Histogram</h3>
      
      <div className="space-y-4">
        <div className="bg-purple-900/20 border border-purple-800/30 rounded-lg p-4">
          <div className="mb-2">
            <label className="text-rose-400">Red Channel</label>
          </div>
          <ResponsiveContainer width="100%" height={80}>
            <BarChart data={histogramData}>
              <Bar dataKey="red" fill="#fb7185" />
            </BarChart>
          </ResponsiveContainer>
        </div>

        <div className="bg-purple-900/20 border border-purple-800/30 rounded-lg p-4">
          <div className="mb-2">
            <label className="text-green-400">Green Channel</label>
          </div>
          <ResponsiveContainer width="100%" height={80}>
            <BarChart data={histogramData}>
              <Bar dataKey="green" fill="#4ade80" />
            </BarChart>
          </ResponsiveContainer>
        </div>

        <div className="bg-purple-900/20 border border-purple-800/30 rounded-lg p-4">
          <div className="mb-2">
            <label className="text-blue-400">Blue Channel</label>
          </div>
          <ResponsiveContainer width="100%" height={80}>
            <BarChart data={histogramData}>
              <Bar dataKey="blue" fill="#60a5fa" />
            </BarChart>
          </ResponsiveContainer>
        </div>

        <div className="bg-purple-900/20 border border-purple-800/30 rounded-lg p-4">
          <div className="mb-2">
            <label className="text-purple-300">Combined</label>
          </div>
          <ResponsiveContainer width="100%" height={100}>
            <BarChart data={histogramData}>
              <Bar dataKey="red" fill="#fb7185" opacity={0.6} />
              <Bar dataKey="green" fill="#4ade80" opacity={0.6} />
              <Bar dataKey="blue" fill="#60a5fa" opacity={0.6} />
            </BarChart>
          </ResponsiveContainer>
        </div>
      </div>
    </div>
  );
}
