"""
EasyOCR Runner Script for Naghuma Toolbox
Handles OCR processing using EasyOCR library
"""
import sys
import json
import cv2
import numpy as np

def run_ocr(image_path):
    """Run EasyOCR on the given image"""
    try:
        import easyocr
        
        # Initialize EasyOCR reader (English only for now)
        reader = easyocr.Reader(['en'], gpu=False)
        
        # Read image
        image = cv2.imread(image_path)
        if image is None:
            return {
                "success": False,
                "error": "Failed to read image",
                "text": "",
                "confidence": 0.0
            }
        
        # Perform OCR
        results = reader.readtext(image)
        
        # Combine all text
        full_text = []
        total_confidence = 0.0
        
        for detection in results:
            bbox, text, confidence = detection
            full_text.append(text)
            total_confidence += confidence
        
        # Calculate average confidence
        avg_confidence = (total_confidence / len(results) * 100) if results else 0.0
        
        return {
            "success": True,
            "error": "",
            "text": " ".join(full_text),
            "confidence": avg_confidence
        }
        
    except ImportError:
        return {
            "success": False,
            "error": "EasyOCR not installed. Install with: pip install easyocr",
            "text": "",
            "confidence": 0.0
        }
    except Exception as e:
        return {
            "success": False,
            "error": str(e),
            "text": "",
            "confidence": 0.0
        }

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print(json.dumps({
            "success": False,
            "error": "No image path provided",
            "text": "",
            "confidence": 0.0
        }))
        sys.exit(1)
    
    image_path = sys.argv[1]
    result = run_ocr(image_path)
    print(json.dumps(result))
