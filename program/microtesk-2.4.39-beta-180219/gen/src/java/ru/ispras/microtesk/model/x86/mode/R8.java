/*
 * Copyright (c) Institute for System Programming of the Russian Academy of Sciences
 * All Rights Reserved
 *
 * Institute for System Programming of the Russian Academy of Sciences (ISP RAS)
 * 25 Alexander Solzhenitsyn st., Moscow, 109004, Russia
 * http://www.ispras.ru
 *
 * The present file was automatically generated on the basis of formal specifications.
 * It is distributed under the same terms and conditions as the derived specifications.
 *
 * N.B. PLEASE DO NOT MODIFY THIS FILE.
 */

package ru.ispras.microtesk.model.x86.mode;

import java.util.Map;
import java.math.BigInteger;
import ru.ispras.microtesk.model.Execution;
import ru.ispras.microtesk.model.ProcessingElement;
import ru.ispras.microtesk.model.data.*;
import ru.ispras.microtesk.model.*;
import ru.ispras.microtesk.model.memory.Location;
import ru.ispras.microtesk.model.x86.PE;
import ru.ispras.microtesk.model.x86.TempVars;

import static ru.ispras.microtesk.model.x86.TypeDefs.*;

public final class R8 extends IsaPrimitive {
  public static final String NAME = "R8";

  private static final class Info extends IsaPrimitiveInfoAnd {
    Info() {
      super(
          IsaPrimitiveKind.MODE,
          "R8",
          R8.class,
          Type.CARD(8)
          );
      addArgument("i", INDEX);
      addArgument("j", BIT);
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final Immediate i = (Immediate) getArgument("i", args);
      final Immediate j = (Immediate) getArgument("j", args);
      return new R8(i, j);
    }
  }

  public static final IsaPrimitiveInfoAnd INFO = new Info();

  public final Immediate i;
  public final Immediate j;

  public R8(final Immediate i, final Immediate j) {
    this.i = i;
    this.j = j;

    addArgument("i", i);
    addArgument("j", j);
  }

  @Override
  public String syntax(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("%s", (i.access().load().equals(Data.valueOf(INDEX, 0x0)) && j.access().load().equals(Data.valueOf(BIT, 0x0)) ? "AL" : (i.access().load().equals(Data.valueOf(INDEX, 0x0)) && j.access().load().equals(Data.valueOf(BIT, 0x1)) ? "AH" : (i.access().load().equals(Data.valueOf(INDEX, 0x1)) && j.access().load().equals(Data.valueOf(BIT, 0x0)) ? "CL" : (i.access().load().equals(Data.valueOf(INDEX, 0x1)) && j.access().load().equals(Data.valueOf(BIT, 0x1)) ? "CH" : (i.access().load().equals(Data.valueOf(INDEX, 0x2)) && j.access().load().equals(Data.valueOf(BIT, 0x0)) ? "DL" : (i.access().load().equals(Data.valueOf(INDEX, 0x2)) && j.access().load().equals(Data.valueOf(BIT, 0x1)) ? "DH" : (i.access().load().equals(Data.valueOf(INDEX, 0x3)) && j.access().load().equals(Data.valueOf(BIT, 0x0)) ? "BL" : (i.access().load().equals(Data.valueOf(INDEX, 0x3)) && j.access().load().equals(Data.valueOf(BIT, 0x1)) ? "BH" : "AL")))))))));
  }

  @Override
  public String image(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("%s", (i.access().load().equals(Data.valueOf(INDEX, 0x0)) && j.access().load().equals(Data.valueOf(BIT, 0x0)) ? "000" : (i.access().load().equals(Data.valueOf(INDEX, 0x0)) && j.access().load().equals(Data.valueOf(BIT, 0x1)) ? "100" : (i.access().load().equals(Data.valueOf(INDEX, 0x1)) && j.access().load().equals(Data.valueOf(BIT, 0x0)) ? "001" : (i.access().load().equals(Data.valueOf(INDEX, 0x1)) && j.access().load().equals(Data.valueOf(BIT, 0x1)) ? "101" : (i.access().load().equals(Data.valueOf(INDEX, 0x2)) && j.access().load().equals(Data.valueOf(BIT, 0x0)) ? "010" : (i.access().load().equals(Data.valueOf(INDEX, 0x2)) && j.access().load().equals(Data.valueOf(BIT, 0x1)) ? "110" : (i.access().load().equals(Data.valueOf(INDEX, 0x3)) && j.access().load().equals(Data.valueOf(BIT, 0x0)) ? "011" : (i.access().load().equals(Data.valueOf(INDEX, 0x3)) && j.access().load().equals(Data.valueOf(BIT, 0x1)) ? "111" : "000")))))))));
  }

  @Override
  public Location access(final ProcessingElement procElem, final TemporaryVariables tempVars) {
    final PE pe__ = (PE) procElem;
    final TempVars vars__ = (TempVars) tempVars;
    return annotate(new Location((j.access().load().equals(Data.valueOf(BIT, 0x0)) ? pe__.GPR.access(i.access().load()).bitField(7, 0).load() : pe__.GPR.access(i.access().load()).bitField(15, 8).load())), vars__);
  }
}