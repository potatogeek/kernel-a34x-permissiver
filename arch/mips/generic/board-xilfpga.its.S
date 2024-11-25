/ {
	images {
<<<<<<< HEAD
		fdt@xilfpga {
=======
		fdt-xilfpga {
>>>>>>> upstream/android-13
			description = "MIPSfpga (xilfpga) Device Tree";
			data = /incbin/("boot/dts/xilfpga/nexys4ddr.dtb");
			type = "flat_dt";
			arch = "mips";
			compression = "none";
<<<<<<< HEAD
			hash@0 {
=======
			hash {
>>>>>>> upstream/android-13
				algo = "sha1";
			};
		};
	};

	configurations {
<<<<<<< HEAD
		conf@xilfpga {
			description = "MIPSfpga Linux kernel";
			kernel = "kernel@0";
			fdt = "fdt@xilfpga";
=======
		conf-xilfpga {
			description = "MIPSfpga Linux kernel";
			kernel = "kernel";
			fdt = "fdt-xilfpga";
>>>>>>> upstream/android-13
		};
	};
};
