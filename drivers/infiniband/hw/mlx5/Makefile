<<<<<<< HEAD
obj-$(CONFIG_MLX5_INFINIBAND)	+= mlx5_ib.o

mlx5_ib-y :=	main.o cq.o doorbell.o qp.o mem.o srq.o mr.o ah.o mad.o gsi.o ib_virt.o cmd.o cong.o
mlx5_ib-$(CONFIG_INFINIBAND_ON_DEMAND_PAGING) += odp.o
mlx5_ib-$(CONFIG_MLX5_ESWITCH) += ib_rep.o
mlx5_ib-$(CONFIG_INFINIBAND_USER_ACCESS) += devx.o
mlx5_ib-$(CONFIG_INFINIBAND_USER_ACCESS) += flow.o
=======
# SPDX-License-Identifier: GPL-2.0-only
obj-$(CONFIG_MLX5_INFINIBAND) += mlx5_ib.o

mlx5_ib-y := ah.o \
	     cmd.o \
	     cong.o \
	     counters.o \
	     cq.o \
	     dm.o \
	     doorbell.o \
	     gsi.o \
	     ib_virt.o \
	     mad.o \
	     main.o \
	     mem.o \
	     mr.o \
	     qp.o \
	     qpc.o \
	     restrack.o \
	     srq.o \
	     srq_cmd.o \
	     wr.o

mlx5_ib-$(CONFIG_INFINIBAND_ON_DEMAND_PAGING) += odp.o
mlx5_ib-$(CONFIG_MLX5_ESWITCH) += ib_rep.o
mlx5_ib-$(CONFIG_INFINIBAND_USER_ACCESS) += devx.o \
					    fs.o \
					    qos.o \
					    std_types.o
>>>>>>> upstream/android-13
