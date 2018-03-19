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

public final class RSEG16 extends IsaPrimitive {
  public static final String NAME = "RSEG16";

  private static final class Info extends IsaPrimitiveInfoAnd {
    Info() {
      super(
          IsaPrimitiveKind.MODE,
          "RSEG16",
          RSEG16.class,
          WORD
          );
      addArgument("i", INDEX);
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final Immediate i = (Immediate) getArgument("i", args);
      return new RSEG16(i);
    }
  }

  public static final IsaPrimitiveInfoAnd INFO = new Info();

  public final Immediate i;

  public RSEG16(final Immediate i) {
    this.i = i;

    addArgument("i", i);
  }

  @Override
  public String syntax(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("%s", (i.access().load().equals(Data.valueOf(INDEX, 0x0)) ? "ES" : (i.access().load().equals(Data.valueOf(INDEX, 0x1)) ? "CS" : (i.access().load().equals(Data.valueOf(INDEX, 0x2)) ? "SS" : (i.access().load().equals(Data.valueOf(INDEX, 0x3)) ? "DS" : "ES")))));
  }

  @Override
  public String image(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    return String.format("%s", (i.access().load().equals(Data.valueOf(INDEX, 0x0)) ? "00" : (i.access().load().equals(Data.valueOf(INDEX, 0x1)) ? "01" : (i.access().load().equals(Data.valueOf(INDEX, 0x2)) ? "10" : (i.access().load().equals(Data.valueOf(INDEX, 0x3)) ? "11" : "000")))));
  }

  @Override
  public Location access(final ProcessingElement procElem, final TemporaryVariables tempVars) {
    final PE pe__ = (PE) procElem;
    final TempVars vars__ = (TempVars) tempVars;
    return annotate(pe__.RSEG.access(i.access().load()), vars__);
  }
}