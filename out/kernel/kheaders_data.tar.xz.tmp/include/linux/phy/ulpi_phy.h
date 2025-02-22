/* SPDX-License-Identifier: GPL-2.0 */
#include <linux/phy/phy.h>


static inline struct phy
*ulpi_phy_create(struct ulpi *ulpi, const struct phy_ops *ops)
{
	struct phy *phy;
	int ret;

	phy = phy_create(&ulpi->dev, NULL, ops);
	if (IS_ERR(phy))
		return phy;

	ret = phy_create_lookup(phy, "usb2-phy", dev_name(ulpi->dev.parent));
	if (ret) {
		phy_destroy(phy);
		return ERR_PTR(ret);
	}

	return phy;
}


static inline void ulpi_phy_destroy(struct ulpi *ulpi, struct phy *phy)
{
	phy_remove_lookup(phy, "usb2-phy", dev_name(ulpi->dev.parent));
	phy_destroy(phy);
}
